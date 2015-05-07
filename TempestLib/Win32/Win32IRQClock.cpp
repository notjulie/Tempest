
#include "stdafx.h"
#include "Win32.h"
#include "Win32IRQClock.h"

Win32IRQClock::Win32IRQClock(void)
{
	// clear
	irqsGenerated = 0;
	irqsHandled = 0;
	msAccumulator = 0;
	terminated = false;

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
	return irqsGenerated > irqsHandled;
}


void Win32IRQClock::ClearIRQ(void)
{
	if (irqsGenerated > irqsHandled)
		++irqsHandled;
}

void Win32IRQClock::ThreadRun(void)
{
	double msPerIRQ = 1000.0 / IRQS_PER_SECOND;

	lastTick = GetTickCount();

	while (!terminated)
	{
		// pause as small a tick as we can
		Sleep(1);

		// add the elapsed time to our time accumulator
		DWORD newTime = GetTickCount();
		DWORD elapsed = newTime - lastTick;
		lastTick = newTime;
		msAccumulator += elapsed;

		// increment the number of IRQs we need to generate
		while (msAccumulator > msPerIRQ)
		{
			msAccumulator -= msPerIRQ;
			++irqsGenerated;
		}
	}
}
