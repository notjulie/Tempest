
#include "stdafx.h"
#include "Win32RealTimeClock.h"

Win32RealTimeClock::Win32RealTimeClock(void)
{
	Reset();
}

void Win32RealTimeClock::Reset(void)
{
	// set our current time to zero
	lastTimeCheck = GetTickCount();
	currentTime = 0;
}


void Win32RealTimeClock::Sync(uint64_t busMSCount)
{
	// update our time
	DWORD now = GetTickCount();
	DWORD elapsed = now - lastTimeCheck;
	lastTimeCheck = now;
	currentTime += elapsed;

	// if Tempest's CPU/bus clock has gotten ahead of us by 8ms
	// or more we need to pause so that real time can catch up to
	// the fake 6502
	if (busMSCount > currentTime + 8)
		Sleep(14);
}

