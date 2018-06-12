
#ifndef PISCREEN_H
#define PISCREEN_H

#include "EGL/egl.h"

#include "SimpleVector.h"


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
   VGPaint GetStroke(uint8_t r, uint8_t g, uint8_t b);
   void StartFrame(void);
   void EndFrame(void);
   void DisplayVector(const SimpleVector &vector);
   void init_ogl(void);
   void DrawDot(float x, float y, uint8_t r, uint8_t g, uint8_t b);

private:
   typedef std::map<uint32_t, VGPaint> StrokeMap;

private:
   STATE_T state;
   StrokeMap strokeMap;
   VGPath dotPath;
   std::vector<VGPath> linePaths;
};

#endif

