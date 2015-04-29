

#include "stdafx.h"

#include "TempestLib/CPU6502.h"
#include "TempestLib/TempestBus.h"
#include "TempestLib/Win32/Win32PerformanceCounter3KHzClock.h"

#include "Tempest.h"


using namespace System;

namespace TempestDotNET {
	Tempest::Tempest(void)
	{
		terminated = false;

		clock = new Win32PerformanceCounter3KHzClock();
		tempestBus = new TempestBus(clock);
		cpu6502 = new CPU6502(tempestBus);
	}

	Tempest::~Tempest(void)
	{
		// stop the thread
		terminated = true;
		thread->Join();

		// delete
		delete cpu6502, cpu6502 = NULL;
		delete tempestBus, tempestBus = NULL;
		delete clock, clock = NULL;
	}

	void Tempest::Start(void)
	{
		thread = gcnew Thread(gcnew ThreadStart(this, &Tempest::ThreadEntry));
		thread->Name = "TempestThread";
		thread->Start();
	}


	void Tempest::ThreadEntry(void)
	{
		cpu6502->Reset();
		while (!terminated)
			cpu6502->SingleStep();
	}
}

