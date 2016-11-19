
#ifndef LS174_H
#define LS174_H

#include "LogicSignal.h"

class LS174
{
public:
   LS174(void);

   LogicInput &Clock(void) { return clock; }
   LogicInput &_Clear(void) { return _clear; }
   LogicInput &GetInput(int n) { return inputs[n - 1]; }
   LogicOutput &GetOutput(int n) { return outputs[n - 1]; }

   const std::string &GetName(void) const { return name; }
   void SetName(const std::string s) { name = s; }

private:
   std::string name;

   LogicSignal clock;
   LogicSignal _clear;
   LogicSignal inputs[8];
   LogicSignal outputs[8];
};

#endif
