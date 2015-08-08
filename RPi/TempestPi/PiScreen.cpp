
#include "stdafx.h"

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
   for (int i=0; i<vectors.size(); ++i)
      DisplayVector(vectors[i]);
   EndFrame();
}

void PiScreen::DisplayVector(const SimpleVector &vector)
{
   float stroke[4] = {0,0,1,1};
   Line(
        (vector.startX + 32768) * state.screen_width / 65536,
        (vector.startY + 32768) * state.screen_width / 65536,
        (vector.endX + 32768) * state.screen_width / 65536,
        (vector.endY + 32768) * state.screen_width / 65536,
        1,
        stroke
        );
}

void PiScreen::Line(float x1, float y1, float x2, float y2, float sw, float stroke[4])
{
    VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
    vguLine(path, x1, y1, x2, y2);
    setstroke(stroke, sw);
    vgDrawPath(path, VG_STROKE_PATH);
    vgDestroyPath(path);

}

void PiScreen::setstroke(float color[4], float width) {
    VGPaint strokePaint = vgCreatePaint();
    vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, color);
    vgSetPaint(strokePaint, VG_STROKE_PATH);
    vgSetf(VG_STROKE_LINE_WIDTH, width);
    vgSeti(VG_STROKE_CAP_STYLE, VG_CAP_BUTT);
    vgSeti(VG_STROKE_JOIN_STYLE, VG_JOIN_MITER);
    vgDestroyPaint(strokePaint);
}
