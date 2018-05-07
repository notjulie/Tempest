
#ifndef ASTEROIDSVECTORINTERPRETER_H
#define ASTEROIDSVECTORINTERPRETER_H

#include "TempestIO/Vector/SimpleVector.h"

class AsteroidsVectorInterpreter
{
public:
   void Interpret(void);
   void SetVectorRAM(const void *vectorRAM);
   void GetAllVectors(std::vector<SimpleVector> &vectors);

private:
   void Draw(int dx, int dy, uint8_t intensity, int scale);
   bool SingleStep(void);
   uint8_t GetAt(uint16_t offsetFromPC);
   bool ClipEndPoint(int &startX, int &startY, int &endX, int &endY);

private:
   uint8_t vectorRAM[0x800];
   bool resetRequested = true;
   bool goRequested = false;
   uint16_t PC = 0;
   int stackIndex = 0;
   bool isHalt = false;
   float x, y;
   uint16_t stack[100];
   std::vector<SimpleVector> vectors;
   int globalScale = 0;
};

#endif
