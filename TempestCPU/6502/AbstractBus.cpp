
#include "stdafx.h"

#include "AbstractBus.h"

AbstractBus::AbstractBus(void)
{
   totalClockCycles = 0;
   nextTimerTime = (uint64_t)(int64_t)-1;
}


AbstractBus::~AbstractBus(void)
{
}

void AbstractBus::IncrementClockCycleCount(int clockCycles)
{
   totalClockCycles += clockCycles;
   if (totalClockCycles >= nextTimerTime)
   {
      for (unsigned i = 0; i < timers.size(); ++i)
      {
         if (totalClockCycles >= timers[i].nextIteration)
         {
            timers[i].timerFunction(this);
            timers[i].nextIteration += timers[i].period;
         }
      }

      UpdateTimers();
   }
}

uint64_t AbstractBus::GetTotalClockCycles(void)
{
   return totalClockCycles;
}

void AbstractBus::StartTimer(uint32_t period, TimerFunction *f)
{
   BusTimer timer;
   timer.period = period;
   timer.nextIteration = totalClockCycles + period;
   timer.timerFunction = f;
   timers.push_back(timer);

   UpdateTimers();
}

void AbstractBus::UpdateTimers(void)
{
   uint64_t soonestTimerTime = (uint64_t)(int64_t)-1;
   for (unsigned i = 0; i < timers.size(); ++i)
      if (timers[i].nextIteration < soonestTimerTime)
         soonestTimerTime = timers[i].nextIteration;
   nextTimerTime = soonestTimerTime;
}
