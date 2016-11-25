
#ifndef ASTEROIDSVECTORMEMORY_H
#define ASTEROIDSVECTORMEMORY_H

#include "LogicSignal.h"

class AsteroidsVectorMemory 
{
public:
   AsteroidsVectorMemory(void);

   LogicOutput &GetOutput(int i) { return outputs[i]; }
   LogicInput &GetLoadAddressInput(int i) { return loadAddressInputs[i]; }
   LogicInput &CounterClock(void) { return counterClock; }
   LogicInput &_CounterLoad(void) { return _counterLoad; }
   LogicInput &_StackRead(void) { return _stackRead; }
   LogicInput &AddressLowBit(void) { return addressLowBit; }
   void SetVectorRAM(const void *vectorRAM, int size);

   uint16_t GetPCWordAddress(void) const { return counterValue; }
   uint16_t GetPCByteAddress(void) const;

private:
   void SetCounterValue(uint16_t newPC);
   void UpdatePC(void);
   void UpdateTopOfStack(void);
   void UpdateOutputs(void);

private:
   uint16_t counterValue = 0;
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
   LogicSignal addressLowBit;
};


#endif
