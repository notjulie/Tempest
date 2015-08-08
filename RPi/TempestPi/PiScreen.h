
#ifndef PISCREEN_H
#define PISCREEN_H

#include "EGL/egl.h"

#include "TempestIO/Vector/SimpleVector.h"


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
   void DisplayVectors(const std::vector<SimpleVector> &vectors);

private:
   void Line(float x1, float y1, float x2, float y2, float sw, float stroke[4]);
   void StartFrame(void);
   void EndFrame(void);
   void DisplayVector(const SimpleVector &vector);
   void init_ogl(void);
   void setstroke(float color[4], float width);

private:
   STATE_T state;
};

#endif

