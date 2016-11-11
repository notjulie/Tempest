
#ifndef NANDGATE_H
#define NANDGATE_H

#include "LogicSignal.h"

class NANDGate : public LogicOutput
{
public:
   void AddSource(LogicOutput &source);

private:
   void UpdateValue(void);

private:
   std::vector<LogicOutput *> sources;
};

#endif
