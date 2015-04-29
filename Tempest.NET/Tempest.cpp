

#include "stdafx.h"
#include <msclr\lock.h>

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
		synchronizer = gcnew Object();
		vectorData = new VectorData();

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
		delete vectorData, vectorData = NULL;
	}

	String ^Tempest::GetProcessorStatus(void)
	{
		return processorStatus;
	}

	String ^Tempest::GetMathBoxStatus(void)
	{
		return gcnew String(tempestBus->GetMathBoxStatus().c_str());
	}

	void Tempest::LoadROM(array<Byte>^ rom, int address)
	{
		std::vector<uint8_t> romCopy;
		for (int i = 0; i < rom->Length; ++i)
			romCopy.push_back(rom[i]);
		tempestBus->LoadROM(&romCopy[0], romCopy.size(), address);
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
			// reset the CPU
			cpu6502->Reset();

			// run
			while (!terminated)
			{
				// run the processor for a little bit
				for (int i = 0; i < 100; ++i)
					cpu6502->SingleStep();

				// pop the new vector data if we have any
				if (tempestBus->HaveNewVectorData())
				{
					msclr::lock l(synchronizer);
					tempestBus->PopVectorData(*vectorData);
				}
			}
		}
		catch (CPU6502Exception &_x6502)
		{
			processorStatus = gcnew String(_x6502.what());
		}
	}
}

