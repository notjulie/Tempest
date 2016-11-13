
#ifndef ORGATE_H
#define ORGATE_H

#include "LogicSignal.h"

class ORGate : public LogicOutput
{
public:
   void AddSource(LogicOutput &source);

private:
   void UpdateValue(void);

private:
   std::vector<LogicOutput *> sources;
};

#endif
