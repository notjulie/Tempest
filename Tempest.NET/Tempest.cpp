

#include "stdafx.h"

#include "TempestLib/6502/CPU6502.h"
#include "TempestLib/6502/CPU6502Exception.h"
#include "TempestLib/TempestBus.h"
#include "TempestLib/Win32/Win32PerformanceCounter3KHzClock.h"

#include "Tempest.h"


using namespace System;

namespace TempestDotNET {
	Tempest::Tempest(void)
	{
		// clear
		terminated = false;
		processorStatus = gcnew String("OK");

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

	String ^Tempest::GetProcessorStatus(void)
	{
		return processorStatus;
	}

	void Tempest::Start(void)
	{
		thread = gcnew Thread(gcnew ThreadStart(this, &Tempest::ThreadEntry));
		thread->Name = "TempestThread";
		thread->Start();
	}


	void Tempest::ThreadEntry(void)
	{
		try
		{
			cpu6502->Reset();
			while (!terminated)
				cpu6502->SingleStep();
		}
		catch (CPU6502Exception &_x6502)
		{
			processorStatus = gcnew String(_x6502.what());
		}
	}
}

