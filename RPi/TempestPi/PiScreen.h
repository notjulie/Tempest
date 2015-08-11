
#ifndef PISCREEN_H
#define PISCREEN_H

#include "EGL/egl.h"

class PiVector;

class PiScreen
{
private:
   typedef struct
   {
       uint32_t screen_width;
       uint32_t screen_height;
   // OpenGL|ES objects
       EGLDisplay display;
       EGLSurface surface;
       EGLContext context;
   } STATE_T;

public:
   PiScreen(void);
   ~PiScreen(void);

   void DisplayVectors(const std::vector<PiVector> &vectors);
   uint32_t GetHeight(void) const { return state.screen_height; }
   void SetColor(int color);

private:
   void StartFrame(void);
   void EndFrame(void);
   void DisplayVector(const PiVector &vector);
   void init_ogl(void);
   void createStroke(const float color[4]);

private:
   STATE_T state;
   std::vector<VGPaint> strokes;
};

#endif

