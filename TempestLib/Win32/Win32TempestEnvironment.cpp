
#include "stdafx.h"
#include "Win32TempestEnvironment.h"

Win32TempestEnvironment::Win32TempestEnvironment(void)
{
	Reset();
}

Win32TempestEnvironment::~Win32TempestEnvironment(void)
{
}

void Win32TempestEnvironment::Reset(void)
{
	// set our current time to zero
	lastTimeCheck = GetTickCount();
	currentTime = 0;
	lastAudioUpdateBusTime = 0;
}


void Win32TempestEnvironment::Sleep(int ms)
{
	::Sleep((DWORD)ms);
}

void Win32TempestEnvironment::SynchronizeClock(uint64_t busMSCount)
{
	// update our time
	DWORD now = GetTickCount();
	DWORD elapsed = now - lastTimeCheck;
	lastTimeCheck = now;
	currentTime += elapsed;

	// let the audio output object update... if it doesn't update frequently it
	// can completely miss very short sounds, of which there are many
	audioOut.Update((int)(busMSCount - lastAudioUpdateBusTime));
	lastAudioUpdateBusTime = busMSCount;

	// if Tempest's CPU/bus clock has gotten ahead of us by 8ms
	// or more we need to pause so that real time can catch up to
	// the fake 6502
	if (busMSCount > currentTime + 8)
		Sleep(14);
}

