
#ifndef LS109_H
#define LS109_H

#include "LogicSignal.h"

class LS109
{
public:
   LS109(void);

   LogicInput &J(void) { return j; }
   LogicInput &K(void) { return k; }
   LogicInput &_CLR(void) { return _clr; }
   LogicInput &_PRE(void) { return _pre; }
   LogicInput &Clock(void) { return clock; }

   LogicOutput &Q(void) { return q; }
   LogicOutput &_Q(void) { return _q; }

private:
   LogicSignal j;
   LogicSignal k;
   LogicSignal _clr;
   LogicSignal _pre;
   LogicSignal clock;

   LogicSignal q;
   LogicSignal _q;
};

#endif
