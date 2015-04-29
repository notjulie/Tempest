

#include "stdafx.h"

#include "TempestLib/CPU6502.h"
#include "TempestLib/TempestBus.h"
#include "TempestLib/Win32/Win32PerformanceCounter3KHzClock.h"

#include "Tempest.h"


using namespace System;

namespace TempestDotNET {
	Tempest::Tempest(void)
	{
		clock = new Win32PerformanceCounter3KHzClock();
		tempestBus = new TempestBus(clock);
		cpu6502 = new CPU6502(tempestBus);
	}

	Tempest::~Tempest(void)
	{
		delete cpu6502, cpu6502 = NULL;
		delete tempestBus, tempestBus = NULL;
		delete clock, clock = NULL;
	}

	void Tempest::Start(void)
	{
		throw gcnew NotImplementedException("Tempest::Start");
	}
}

