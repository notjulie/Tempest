
#ifndef LS42_H
#define LS42_H

#include "LogicSignal.h"

class LS42
{
public:
   LS42(void);

   LogicInput &A(void) { return a; }
   LogicInput &B(void) { return b; }
   LogicInput &C(void) { return c; }
   LogicInput &D(void) { return d; }
   LogicOutput &GetOutput(int n) { return outputs[n]; }

   uint8_t GetInput(void) const;

private:
   void UpdateValue(void);

private:
   LogicSignal a;
   LogicSignal b;
   LogicSignal c;
   LogicSignal d;
   LogicSignal outputs[10];
};


#endif
