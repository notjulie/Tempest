
#ifndef ANDGATE_H
#define ANDGATE_H

#include "LogicSignal.h"

class ANDGate : public LogicOutput
{
public:
   void AddSource(LogicOutput &source);

private:
   void UpdateValue(void);

private:
   std::vector<LogicOutput *> sources;
};

#endif
