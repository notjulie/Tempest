// shapes: minimal program to explore OpenVG
// Anthony Starks (ajstarks@gmail.com)
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "bcm_host.h"

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "EGL/egl.h"
#include "GLES/gl.h"

typedef struct
{
    uint32_t screen_width;
    uint32_t screen_height;
// OpenGL|ES objects
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
} STATE_T;

static void exit_func(void);
static STATE_T _state, *state=&_state;

// init_ogl sets the display, OpenGL|ES context and screen information
// state holds the OGLES model information
static void init_ogl(STATE_T *state) {
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
    state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(state->display!=EGL_NO_DISPLAY);

// initialize the EGL display connection
    result = eglInitialize(state->display, NULL, NULL);
    assert(EGL_FALSE != result);

// bind OpenVG API
    eglBindAPI(EGL_OPENVG_API);

// get an appropriate EGL frame buffer configuration
    result = eglChooseConfig(state->display, attribute_list, &config, 1, &num_config);
    assert(EGL_FALSE != result);

// create an EGL rendering context
    state->context = eglCreateContext(state->display, config, EGL_NO_CONTEXT, NULL);
    assert(state->context!=EGL_NO_CONTEXT);

// create an EGL window surface
    success = graphics_get_display_size(0 /* LCD */, &state->screen_width, &state->screen_height);
    assert( success >= 0 );

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = state->screen_width;
    dst_rect.height = state->screen_height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = state->screen_width << 16;
    src_rect.height = state->screen_height << 16;

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
        0/*layer*/, &dst_rect, 0/*src*/,
        &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/,
        DISPMANX_NO_ROTATE);

    nativewindow.element = dispman_element;
    nativewindow.width = state->screen_width;
    nativewindow.height = state->screen_height;
    vc_dispmanx_update_submit_sync( dispman_update );

    state->surface = eglCreateWindowSurface( state->display, config, &nativewindow, NULL );
    assert(state->surface != EGL_NO_SURFACE);

// connect the context to the surface
    result = eglMakeCurrent(state->display, state->surface, state->surface, state->context);
    assert(EGL_FALSE != result);

//DAVE - Set up screen ratio
    glViewport(0, 0, (GLsizei)state->screen_width, (GLsizei)state->screen_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float ratio = (float)state->screen_width / (float)state->screen_height;
    glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1.0f, 10.0f);
}


// setfill sets the fill color
void setfill(float color[4]) {
    VGPaint fillPaint = vgCreatePaint();
    vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, color);
    vgSetPaint(fillPaint, VG_FILL_PATH);
    vgDestroyPaint(fillPaint);
}


// setstroke sets the stroke color and width
void setstroke(float color[4], float width) {
    VGPaint strokePaint = vgCreatePaint();
    vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, color);
    vgSetPaint(strokePaint, VG_STROKE_PATH);
    vgSetf(VG_STROKE_LINE_WIDTH, width);
    vgSeti(VG_STROKE_CAP_STYLE, VG_CAP_BUTT);
    vgSeti(VG_STROKE_JOIN_STYLE, VG_JOIN_MITER);
    vgDestroyPaint(strokePaint);
}


// rect makes a rectangle  at the specified location and dimensions, applying style
void Rect(float x, float y, float w, float h, float sw, float fill[4], float stroke[4]) {
    VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
    vguRect(path, x, y, w, h);
    setfill(fill);
    setstroke(stroke, sw);
    vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
    vgDestroyPath(path);
}


// Line makes a line, applying style
void Line(float x1, float y1, float x2, float y2, float sw, float stroke[4]) {
    VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
    vguLine(path, x1, y1, x2, y2);
    setstroke(stroke, sw);
    vgDrawPath(path, VG_STROKE_PATH);
    vgDestroyPath(path);

}


// Roundrect makes an rounded rectangle at the specified location and dimensions, applying style
void Roundrect(float x, float y, float w, float h, float rw, float rh, float sw, float fill[4], float stroke[4]) {
    VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
    vguRoundRect(path, x, y, w, h, rw, rh);
    setfill(fill);
    setstroke(stroke, sw);
    vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
    vgDestroyPath(path);
}


// Ellipse makes an ellipse at the specified location and dimensions, applying style
void Ellipse(float x, float y, float w, float h, float sw, float fill[4], float stroke[4]) {
    VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
    vguEllipse(path, x, y, w, h);
    setfill(fill);
    setstroke(stroke, sw);
    vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
    vgDestroyPath(path);
}


// Start begins the picture, clearing a rectangluar region with a specified color
void Start(int width, int height, float fill[4]) {
    vgSetfv(VG_CLEAR_COLOR, 4, fill);
    vgClear(0, 0, width, height);
    vgLoadIdentity();
}


// End checks for errors, and renders to the display
void End() {
    assert(vgGetError() == VG_NO_ERROR);
    eglSwapBuffers(state->display, state->surface);
    assert(eglGetError() == EGL_SUCCESS);
}


// randcolor returns a fraction of 255
VGfloat randcolor() {
    return (VGfloat)(rand() % 256) / 255.0;
}


// randf returns a floating point number bounded by n
VGfloat randf(int n) {
    return (VGfloat)(rand() % n);
}


// rshapes draws shapes (rect and ellipse) with random colors, strokes, and sizes.
void rshapes(int width, int height, int n) {
    float rcolor[4], scolor[4], bgcolor[4] = {0,0,0,1};
    scolor[3] = 1;                                // strokes are solid
    Start(width, height, bgcolor);
    int i;
    for (i=0; i < n; i++) {
        rcolor[0] = randcolor();
        rcolor[1] = randcolor();
        rcolor[2] = randcolor();
        rcolor[3] = randcolor();

        scolor[1] = randcolor();
        scolor[2] = randcolor();
        scolor[3] = randcolor();
        if (i%2 == 0) {
            Ellipse(randf(width), randf(height), randf(200), randf(100), randf(10), rcolor, scolor);
        }
        else {
            Rect(randf(width), randf(height), randf(200), randf(100), randf(10), rcolor, scolor);
        }
    }
    End();
}


// exit_func cleans up
static void exit_func(void) {
// clear screen
    glClear( GL_COLOR_BUFFER_BIT );
    eglSwapBuffers(state->display, state->surface);

// Release OpenGL resources
    eglMakeCurrent( state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    eglDestroySurface( state->display, state->surface );
    eglDestroyContext( state->display, state->context );
    eglTerminate( state->display );
}


// main initializes the system and shows the picture,
// exit and clean up when you hit [RETURN].
int main (int argc, char **argv) {
    int w, h, n;
    bcm_host_init();
    memset( state, 0, sizeof( *state ) );         // clear application state
    init_ogl(state);                              // Start OGLES
    w = state->screen_width;
    h = state->screen_height;
    n = 50;
    if (argc > 1) {
        n = atoi(argv[1]);
    }

    srand ( time(NULL) );

    for (int i=0; i<100; ++i)
        rshapes(w, h, n);
    while (getchar() != '\n') {
        ;
    }
    exit_func();
    return 0;
}
