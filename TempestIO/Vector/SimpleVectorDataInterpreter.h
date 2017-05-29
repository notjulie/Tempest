
#ifndef SIMPLEVECTORDATAINTERPRETER_H
#define SIMPLEVECTORDATAINTERPRETER_H

#include "SimpleVector.h"
#include "VectorDataInterpreter.h"

class SimpleVectorDataInterpreter : public VectorDataInterpreter
{
public:
	SimpleVectorDataInterpreter(void);


private:
   void Char(char c);
};

class SimpleVectorGenerator : public VectorGenerator
{
public:
   void GetAllVectors(std::vector<SimpleVector> &vectors) { vectors = this->vectors; }

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

#endif
