
#ifndef INVERTER_H
#define INVERTER_H

#include "LogicSignal.h"


class Inverter : public LogicInput, public LogicOutput
{
public:
   void Set(bool inputValue) { LogicOutput::SetOutputValue(!inputValue); }
};


#endif
