
#ifndef LS74_H
#define LS74_h

#include "LogicSignal.h"

class LS74 : public LogicOutput
{
public:
   LS74(void);

   LogicInput &_CLR(void) { return _clr; }
   LogicInput &Clock(void) { return clock; }
   LogicInput &D(void) { return d; }
   LogicInput &_PRE(void) { return _pre; }

private:
   void HandleLoadAndClear(void);

private:
   LogicSignal clock;
   LogicSignal d;
   LogicSignal _clr;
   LogicSignal _pre;
};

#endif
