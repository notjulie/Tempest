
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
   ~PiScreen(void);

   void DisplayVectors(const std::vector<SimpleVector> &vectors);

private:
   void DeleteStrokes(void);
   VGPaint GetStroke(const TempestColor &color);
   void StartFrame(void);
   void EndFrame(void);
   void DisplayVector(const SimpleVector &vector);
   void init_ogl(void);

private:
   typedef std::map<uint32_t, VGPaint> StrokeMap;

private:
   STATE_T state;
   StrokeMap strokeMap;
   VGPath dotPath;
   std::vector<VGPath> linePaths;
};

#endif

