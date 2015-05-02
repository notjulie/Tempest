
#include "stdafx.h"
#include "Win32.h"

#include "..\TempestException.h"

#include "Win32PerformanceCounter3KHzClock.h"

Win32PerformanceCounter3KHzClock::Win32PerformanceCounter3KHzClock(void)
{
   LARGE_INTEGER liFrequency;
   if (!QueryPerformanceFrequency(&liFrequency))
      throw TempestException("QueryPerformanceFrequency failed");
   performanceCountsPerHalfCycle = (uint64_t)liFrequency.QuadPart / 6000;
}


bool Win32PerformanceCounter3KHzClock::IsHigh(void)
{
   LARGE_INTEGER liCounter;
   if (!QueryPerformanceCounter(&liCounter))
      throw TempestException("QueryPerformanceCounter failed");
   bool result = ((liCounter.QuadPart / performanceCountsPerHalfCycle) & 1) != 0;
   return result;
}
