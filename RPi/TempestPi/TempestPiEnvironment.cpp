
#include "stdafx.h"

#include "TempestCPU/TempestException.h"

#include "TempestPiEnvironment.h"


TempestPiEnvironment::TempestPiEnvironment(void)
{
   realTimeMS = 0;
}


void TempestPiEnvironment::Reset(void)
{
   if (0 != clock_gettime(CLOCK_REALTIME, &startTime))
      throw TempestException("TempestPiEnvironment::Reset error");
   realTimeMS = 0;
}

void TempestPiEnvironment::Sleep(int ms)
{
   usleep(ms * 1000);
}

void TempestPiEnvironment::SynchronizeClock(uint64_t busMSCount)
{
   // get the current time
   timespec now;
   if (0 != clock_gettime(CLOCK_REALTIME, &now))
      throw TempestException("TempestPiEnvironment::SynchronizeClock error");

   // calculate elapsed milliseconds
   uint64_t elapsedMS = 1000 * (now.tv_sec - startTime.tv_sec);
   elapsedMS += (now.tv_nsec - startTime.tv_nsec) / 1000000;

   // update our counter
   realTimeMS += elapsedMS;
   startTime = now;

   // see how far ahead the bus time is relative to real time
   int32_t msAhead = busMSCount - realTimeMS;

   // if the bus is ahead of realtime then we need to dawdle
   if (msAhead > 0)
   {
      usleep(msAhead * 1000);
      return;
   }

   // if realtime has gotten too far ahead of the bus that means
   // something happened to slow down the 6502 thread; just resynch
   // so that the 6502 doesn't suddenly burst to try to catch up
   if (msAhead < 200)
      realTimeMS = busMSCount;
}

