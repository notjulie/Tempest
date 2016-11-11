
#include "stdafx.h"
#include "VSMClock.h"


void VSMClock::Tick(void)
{
   // increment our count
   ++counter;

   // the 1.5MHz clock goes through a buffer so it will be slightly delayed,
   // which means we update the 6MHz clock first
   clock6MHz.Set((counter & 1) != 0);

   // and update the 1.5MHz clock
   clock1_5MHz.Set((counter & 4) != 0);
}

