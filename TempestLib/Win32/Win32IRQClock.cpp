
#include "stdafx.h"
#include "Win32.h"

#include "TempestException.h"

#include "Win32IRQClock.h"

Win32IRQClock::Win32IRQClock(void)
{
	// clear
	irq = false;
	accumulator = 0;
	terminated = false;

	// get the performance counter frequency
	LARGE_INTEGER liFrequency;
	if (!QueryPerformanceFrequency(&liFrequency))
		throw TempestException("QueryPerformanceFrequency failed");
	performanceCountsPerIRQ = (uint64_t)liFrequency.QuadPart / IRQS_PER_SECOND;

	// start our thread
	thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadStart, this, 0, &threadID);
}

Win32IRQClock::~Win32IRQClock(void)
{
	terminated = true;
	WaitForSingleObject(thread, INFINITE);
}


bool Win32IRQClock::GetIRQ(void)
{
	// we get called really often, hopefully millions of times per second... keep this
	// ridiculously simple
	return irq;
}


void Win32IRQClock::ClearIRQ(void)
{
	irq = false;
}

void Win32IRQClock::ThreadRun(void)
{
	LARGE_INTEGER liCounter;
	if (!QueryPerformanceCounter(&liCounter))
		throw TempestException("QueryPerformanceCounter failed");
	lastPerformanceCount = (uint64_t)liCounter.QuadPart;

	while (!terminated)
	{
		// Sleep(0) is rather unfriendly, but it's better than not calling Sleep
		// at all, and seriously we need the precision.
		Sleep(0);

		// add the elapsed time to our time accumulator
		if (!QueryPerformanceCounter(&liCounter))
			throw TempestException("QueryPerformanceCounter failed");
		uint64_t newTime = (uint64_t)liCounter.QuadPart;
		uint64_t elapsed = newTime - lastPerformanceCount;
		lastPerformanceCount = newTime;
		accumulator += elapsed;

		// increment the number of IRQs we need to generate
		while (accumulator > performanceCountsPerIRQ)
		{
			accumulator -= performanceCountsPerIRQ;
			irq = true;
		}
	}
}
