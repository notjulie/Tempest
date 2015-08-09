
#include "stdafx.h"
#include <time.h>

#include "TempestCPU/TempestException.h"

#include "TempestPiEnvironment.h"


AbstractThread *TempestPiEnvironment::CreateThread(ThreadEntry *entry, void *param)
{
   return new RPiThread(entry, param);
}

void TempestPiEnvironment::Reset(void)
{
   if (0 != clock_gettime(CLOCK_REALTIME, &startTime))
      throw TempestException("TempestPiEnvironment::Reset error");
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

   // dawdle if real time has gottem behind bus time
   if (busMSCount > elapsedMS)
      usleep((busMSCount - elapsedMS) * 1000);
}

