
#ifndef ASTEROIDSVECTORMEMORY_H
#define ASTEROIDSVECTORMEMORY_H

#include "LogicSignal.h"

class AsteroidsVectorMemory 
{
public:
   AsteroidsVectorMemory(void);

   LogicOutput &GetOutput(int i) { return outputs[i]; }
   void SetVectorRAM(const void *vectorRAM, int size);

private:
   void SetPC(uint16_t newPC);
   void UpdatePC(void);
   void UpdateTopOfStack(void);
   void UpdateOutputs(void);

private:
   uint16_t pc = 0;
   uint8_t vectorRAM[0x800];
   int stackPointer = 0;
   uint16_t stack[4];

   LogicSignal counterClock;
   LogicSignal _counterLoad;
   LogicSignal _stackWrite;
   LogicSignal _stackRead;
   LogicSignal _stackPointerIncrement;
   LogicSignal stackPointerClock;
   LogicSignal _enableLoadAddress;
   LogicSignal outputs[8];
   LogicSignal loadAddressInputs[12];
};


#endif
