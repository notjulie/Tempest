
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
      unsigned i = 0;
      while (i < timers.size())
      {
         if (totalClockCycles >= timers[i].time)
         {
            timers[i].timerFunction();
            timers[i] = timers[timers.size() - 1];
            timers.resize(timers.size() - 1);
         }
         else
         {
            ++i;
         }
      }

      UpdateTimers();
   }
}

void AbstractBus::StartTimer(int cycleCount, const std::function<void()> &f)
{
   BusTimer timer;
   timer.time = totalClockCycles + cycleCount;
   timer.timerFunction = f;
   timers.push_back(timer);

   UpdateTimers();
}

void AbstractBus::UpdateTimers(void)
{
   uint64_t soonestTimerTime = (uint64_t)(int64_t)-1;
   for (unsigned i = 0; i < timers.size(); ++i)
      if (timers[i].time < soonestTimerTime)
         soonestTimerTime = timers[i].time;
   nextTimerTime = soonestTimerTime;
}
