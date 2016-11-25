
#ifndef VSMROM_H
#define VSMROM_H

#include "LogicSignal.h"

class VSMROM
{
public:
   VSMROM(void);

   LogicInput &GetInput(int n) { return inputs[n]; }
   LogicOutput &GetOutput(int n) { return outputs[n - 1]; }

   uint8_t GetAddress(void) const;

private:
   void UpdateValue(void);

private:
   LogicSignal inputs[8];
   LogicSignal outputs[4];
};

#endif
