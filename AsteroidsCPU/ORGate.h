
#ifndef ORGATE_H
#define ORGATE_H

#include "LogicSignal.h"

class ORGate : public LogicOutput
{
public:
   void AddSource(LogicOutput &source);

   const std::string &GetName(void) const { return name; }
   void SetName(const std::string s) { name = s; }

private:
   void UpdateValue(void);

private:
   std::string name;
   std::vector<LogicOutput *> sources;
};

#endif
