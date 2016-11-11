
#ifndef LS174_H
#define LS174_H

#include "LogicSignal.h"

class LS174
{
public:
   LS174(void);

   LogicOutput GetOutput(int n) { return outputs[n-1]; }

private:
   LogicSignal clock;
   LogicSignal _clear;
   LogicSignal inputs[6];
   LogicSignal outputs[6];
};

#endif
