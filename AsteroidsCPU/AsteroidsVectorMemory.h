
#ifndef ASTEROIDSVECTORMEMORY_H
#define ASTEROIDSVECTORMEMORY_H

#include "LogicSignal.h"

class AsteroidsVectorMemory 
{
public:
   LogicOutput &GetOutput(int i) { return outputs[i]; }
   void SetVectorRAM(const void *vectorRAM);

private:
   uint8_t vectorRAM[0x800];
   LogicSignal outputs[8];
};


#endif
