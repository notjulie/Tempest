
#include "stdafx.h"

#include "6502/CPU6502Exception.h"
#include "AbstractRealTimeClock.h"
#include "TempestException.h"

#include "TempestRunner.h"

TempestRunner::TempestRunner(AbstractRealTimeClock *realTimeClock)
	:
		tempestBus(),
		cpu6502(&tempestBus)
{
	// save parameters
	this->realTimeClock = realTimeClock;

	// clear
	terminate = false;
	isTerminated = false;
	irqCount = 0;
	totalClockCycles = 0;
	theThread = NULL;
}

TempestRunner::~TempestRunner(void)
{
	if (theThread != NULL)
	{
		terminate = true;
		theThread->join();
		delete theThread, theThread = NULL;
	}
}


void TempestRunner::Start(void)
{
	// create the thread is all
	theThread = new std::thread(RunnerThreadEntry, this);
}


void TempestRunner::RunnerThread(void)
{
	try
	{
		// reset the CPU and the realtime clock
		cpu6502.Reset();

		// synchronize our clock with the real world
		int clockCyclesPer3KHzHalfWave = 1500000 / 6000;
		int clockCyclesPerIRQ = 1500000 / 250;
		int irqTimer = 0;
		realTimeClock->Reset();
		totalClockCycles = 0;

		// run
		while (!terminate)
		{
			// Run the processor for a little bit without any overhead... figuring an average
			// of about 4 cycles per instruction that's about 2.7 uS per instruction, in CPU
			// time.  So the CPU thinks this loop runs for 270us, which is a reasonable burst
			// of effort without checking the various clocks.
			int newClockCycles = 0;
			for (int i = 0; i < 100; ++i)
				newClockCycles += cpu6502.SingleStep();

			// update our master counter
			totalClockCycles += newClockCycles;

			// set the value of the 3KHz clock
			tempestBus.Set3KHzClockState(((totalClockCycles / clockCyclesPer3KHzHalfWave) & 1) != 0);

			// update the IRQ counter
			irqTimer += newClockCycles;
			if (irqTimer >= clockCyclesPerIRQ)
			{
				irqTimer -= clockCyclesPerIRQ;

				// tell the CPU about the IRQ
				cpu6502.IRQ();
				++irqCount;

				// this happens every 4ms in processor time, so it's a good time to synch
				// up with the real world clock
				realTimeClock->Sync(totalClockCycles / 1500);
			}
		}

		processorStatus = "Exited normally";
	}
	catch (CPU6502Exception &_x6502)
	{
		processorStatus = _x6502.what();
	}
	catch (TempestException &_xTempest)
	{
		// for now this goes as the processor status, too
		processorStatus = _xTempest.what();
	}

	isTerminated = true;
}
