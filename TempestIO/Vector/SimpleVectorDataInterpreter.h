
#ifndef SIMPLEVECTORDATAINTERPRETER_H
#define SIMPLEVECTORDATAINTERPRETER_H

#include "SimpleVector.h"
#include "VectorDataInterpreter.h"

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif


class SimpleVectorGenerator : public VectorGenerator
{
public:
   virtual ~SimpleVectorGenerator(void) {}

   void GetAllVectors(std::vector<SimpleVector> &_vectors) { _vectors = this->vectors; }

   virtual void Center(void);
   virtual void LDraw(int x, int y, int intensity);
   virtual void Scale(int binaryScale, int linearScale);
   virtual void SDraw(int x, int y, int intensity);
   virtual void Stat(int colorIndex, int intensity, int color);

private:
   void Char(char c);
   bool ClipEndPoint(int &startX, int &startY, int &endX, int &endY);

private:
   float x = 0.0F;
   float y = 0.0F;
   int binaryScale = 1;
   int linearScale = 0;
   int color = 0;
   std::vector<SimpleVector> vectors;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
