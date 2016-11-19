
#ifndef LS74_H
#define LS74_h

#include "Inverter.h"
#include "LogicSignal.h"

class LS74
{
public:
   LS74(void);

   LogicInput &_CLR(void) { return _clr; }
   LogicInput &Clock(void) { return clock; }
   LogicInput &D(void) { return d; }
   LogicInput &_PRE(void) { return _pre; }
   LogicOutput &Q(void) { return q; }
   LogicOutput &_Q(void) { return _q; }

private:
   void HandleLoadAndClear(void);

private:
   LogicSignal clock;
   LogicSignal d;
   LogicSignal _clr;
   LogicSignal _pre;
   LogicSignal q;
   LogicSignal _q;
};

#endif
