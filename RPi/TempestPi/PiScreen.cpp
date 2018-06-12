
#include "stdafx.h"
#include <math.h>

#include "bcm_host.h"
#include "EGL/egl.h"
#include "GLES/gl.h"
#include "VG/openvg.h"
#include "VG/vgu.h"

#include "PiScreen.h"

PiScreen::PiScreen(void)
{
   bcm_host_init();
   memset(&state, 0, sizeof(state));         // clear application state
   init_ogl();                              // Start OGLES

   // create the path we use for drawing single pixels
   dotPath = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
   vguLine(dotPath, 0, 0, 0, 1);
   for (int i=0; i<1024; ++i)
   {
      VGPath linePath = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
      vguLine(linePath, 0, 0, i, 0);
      linePaths.push_back(linePath);
   }
}

PiScreen::~PiScreen(void)
{
   vgDestroyPath(dotPath);
   DeleteStrokes();
}

void PiScreen::DeleteStrokes(void)
{
   for (StrokeMap::iterator i=strokeMap.begin(); i!=strokeMap.end(); ++i)
      vgDestroyPaint((*i).second);
   strokeMap.clear();
}

VGPaint PiScreen::GetStroke(uint8_t r, uint8_t g, uint8_t b)
{
   // make a key based on the color
   uint32_t key =
      (r << 16) +
      (g << 8) +
      (b);

   // if we already have a stroke for that color, return it
   StrokeMap::iterator existingStroke = strokeMap.find(key);
   if (existingStroke != strokeMap.end())
      return (*existingStroke).second;

   // create a new stroke
   float rgba[] = {
      r / 255.0F,
      g / 255.0F,
      b / 255.0F,
      1.0F
   };
   VGPaint stroke = vgCreatePaint();
   vgSetParameteri(stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
   vgSetParameterfv(stroke, VG_PAINT_COLOR, 4, rgba);

   // add it to the map and return it
   strokeMap[key] = stroke;
   return stroke;
}


// init_ogl sets the display, OpenGL|ES context and screen information
// state holds the OGLES model information
void PiScreen::init_ogl(void) {
    int32_t success = 0;
    EGLBoolean result;
    EGLint num_config;

    static EGL_DISPMANX_WINDOW_T nativewindow;

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    static const EGLint attribute_list[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    EGLConfig config;

// get an EGL display connection
    state.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (state.display==EGL_NO_DISPLAY)
      throw "No display";

// initialize the EGL display connection
    result = eglInitialize(state.display, NULL, NULL);
    if (EGL_FALSE == result)
      throw "eglInitialize error";

// bind OpenVG API
    eglBindAPI(EGL_OPENVG_API);

// get an appropriate EGL frame buffer configuration
    result = eglChooseConfig(state.display, attribute_list, &config, 1, &num_config);
    if (EGL_FALSE == result)
      throw "eglChooseConfig error";

// create an EGL rendering context
    state.context = eglCreateContext(state.display, config, EGL_NO_CONTEXT, NULL);
    if (state.context==EGL_NO_CONTEXT)
      throw "eglCreateContext error";

// create an EGL window surface
    success = graphics_get_display_size(0 /* LCD */, &state.screen_width, &state.screen_height);
    if (success < 0)
      throw "graphics_get_display_size error";

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = state.screen_width;
    dst_rect.height = state.screen_height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = state.screen_width << 16;
    src_rect.height = state.screen_height << 16;

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
        0/*layer*/, &dst_rect, 0/*src*/,
        &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/,
        DISPMANX_NO_ROTATE);

    nativewindow.element = dispman_element;
    nativewindow.width = state.screen_width;
    nativewindow.height = state.screen_height;
    vc_dispmanx_update_submit_sync( dispman_update );

    state.surface = eglCreateWindowSurface( state.display, config, &nativewindow, NULL );
    if (state.surface == EGL_NO_SURFACE)
      throw "eglCreateWindowSurface error";

// connect the context to the surface
    result = eglMakeCurrent(state.display, state.surface, state.surface, state.context);
    if (EGL_FALSE == result)
      throw "eglMakeCurrent error";

//DAVE - Set up screen ratio
    glViewport(0, 0, (GLsizei)state.screen_width, (GLsizei)state.screen_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float ratio = (float)state.screen_width / (float)state.screen_height;
    glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1.0f, 10.0f);
}


void PiScreen::StartFrame(void)
{
   float bgcolor[4] = {0,0,0,1};
   vgSetfv(VG_CLEAR_COLOR, 4, bgcolor);
   vgClear(0, 0, state.screen_width, state.screen_height);
   vgLoadIdentity();
}

void PiScreen::EndFrame(void)
{
   if (vgGetError() != VG_NO_ERROR)
      throw "EndFrame error";
   eglSwapBuffers(state.display, state.surface);
   int error = eglGetError();
   if (error != EGL_SUCCESS)
      throw "EndFrame error";
}

void PiScreen::DisplayVectors(const std::vector<SimpleVector> &vectors)
{
   StartFrame();
   for (unsigned i=0; i<vectors.size(); ++i)
      DisplayVector(vectors[i]);
   EndFrame();
}

void PiScreen::DisplayVector(const SimpleVector &vector)
{
   // calculate our screen coordinates
   float x1 = (float)state.screen_width - (float)(32768 - vector.startY) * state.screen_height / 65536;
   float y1 = (float)(32768 - vector.startX) * state.screen_height / 65536;
   float x2 = (float)state.screen_width - (float)(32768 - vector.endY) * state.screen_height / 65536;
   float y2 = (float)(32768 - vector.endX) * state.screen_height / 65536;

   // center vertically... note that the display is rotated so that X is the vertical
   // axis
   float verticalOffset = 200;
   x1 -= verticalOffset;
   x2 -= verticalOffset;

   // if this is just a dot draw our dot path
   if (vector.startX==vector.endX && vector.startY==vector.endY)
   {
      DrawDot(x1, y1, vector.r, vector.g, vector.b);
      return;
   }

   // set our line drawing parameters
   vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
   vgSetPaint(GetStroke(vector.r, vector.g, vector.b), VG_STROKE_PATH);
   vgSetf(VG_STROKE_LINE_WIDTH, 1);
   vgSeti(VG_STROKE_CAP_STYLE, VG_CAP_BUTT);
   vgSeti(VG_STROKE_JOIN_STYLE, VG_JOIN_MITER);

   // if we have a line of the correct length just transform it to the right location
   float dx = x2 - x1;
   float dy = y2 - y1;
   int length = (int)(sqrtf(dx*dx + dy*dy) + 0.5);
   if (length < (int)linePaths.size())
   {
      vgTranslate(x1, y1);
      vgRotate(180 * atan2f(dy, dx) / 3.1415927);
      vgDrawPath(linePaths[length], VG_STROKE_PATH);
      vgLoadIdentity();
      return;
   }

   // fall back on old school line draw
   VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
   vguLine(path, x1, y1, x2, y2);
   vgDrawPath(path, VG_STROKE_PATH);
   vgDestroyPath(path);
}


void PiScreen::DrawDot(float x, float y, uint8_t r, uint8_t g, uint8_t b)
{
   // In general it appears that openVG on the RPi is most efficient
   // at drawing lines, moreso than filling shapes.  We have a path
   // that we use that consists of a line of length one which we
   // transform to the correct location and draw with a line width of 1.
   // I've also found that 1.75 is a really good scale factor for the line.
   // Smaller than that it is difficult to see the bullets and very difficult
   // to tell when it's time to zap.  Bigger than that and they just seem
   // too much.

   // A useful performance test would be to see if it's quicker to change the
   // length of the line and width of the stroke versus scaling the line.
   vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
   vgSetPaint(GetStroke(r, g, b), VG_STROKE_PATH);
   vgSetf(VG_STROKE_LINE_WIDTH, 1);
   vgSeti(VG_STROKE_CAP_STYLE, VG_CAP_BUTT);
   vgSeti(VG_STROKE_JOIN_STYLE, VG_JOIN_MITER);

   vgTranslate(x, y - 0.5);
   vgScale(1.75, 1.75);
   vgDrawPath(dotPath, VG_STROKE_PATH);
   vgLoadIdentity();
}
