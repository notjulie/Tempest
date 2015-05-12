
#include "stdafx.h"

#include "6502/CPU6502Exception.h"
#include "AbstractTempestEnvironment.h"
#include "TempestException.h"

#include "TempestRunner.h"

TempestRunner::TempestRunner(AbstractTempestEnvironment *_environment)
	:
		tempestBus(),
		cpu6502(&tempestBus)
{
	// save parameters
	environment = _environment;

	// clear
	terminate = false;
	isTerminated = false;
	irqCount = 0;
	totalClockCycles = 0;
	theThread = NULL;
	for (int i = 0; i < 64 * 1024; ++i)
		breakpoints[i] = false;

	// give the environment a reference to the sound stream
	environment->SetSoundStream(GetSoundStream());
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
		environment->Reset();
		totalClockCycles = 0;

		// run
		while (!terminate)
		{
			// Run the processor until the 3KHz clock changes
			int cyclesToRun = (int)(clockCyclesPer3KHzHalfWave - (totalClockCycles % clockCyclesPer3KHzHalfWave));
			int newClockCycles = 0;
			while (newClockCycles < cyclesToRun)
			{
				if (breakpoints[cpu6502.GetPC()])
				{
					while (!terminate)
					{
						environment->Sleep(50);
					}
				}
				newClockCycles += cpu6502.SingleStep();
			}

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
				environment->SynchronizeClock(totalClockCycles / 1500);
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
