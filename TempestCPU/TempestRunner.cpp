
#include "stdafx.h"

#include "6502/CPU6502Exception.h"
#include "AbstractTempestEnvironment.h"
#include "TempestException.h"

#include "TempestRunner.h"

TempestRunner::TempestRunner(AbstractTempestEnvironment *_environment)
	:
		tempestBus(_environment),
		cpu6502(&tempestBus)
{
	// save parameters
	environment = _environment;

	// clear
	state = Unstarted;
	requestedAction = NoAction;
	terminateRequested = false;
	irqCount = 0;
	totalClockCycles = 0;
	theThread = NULL;
	for (int i = 0; i < 64 * 1024; ++i)
		breakpoints[i] = false;
   resetRequested = false;
}

TempestRunner::~TempestRunner(void)
{
	if (theThread != NULL)
	{
		terminateRequested = true;
		delete theThread, theThread = NULL;
	}
}


void TempestRunner::SetDemoMode(void)
{
   // tell the bus to set the proper DIP switches
   tempestBus.SetDemoMode();

   // force a reset
   resetRequested = true;
}

void TempestRunner::Start(void)
{
	// create the thread is all
   theThread = environment->CreateThread(RunnerThreadEntry, this);
}


void TempestRunner::RunnerThread(void)
{
	try
	{
		// set our state
		state = Running;

		// reset the CPU and the realtime clock
		cpu6502.Reset();

		// synchronize our clock with the real world
		int clockCyclesPer3KHzHalfWave = 1500000 / 6000;
		int clockCyclesPerIRQ = 1500000 / 250;
		int irqTimer = 0;
		environment->Reset();
		totalClockCycles = 0;

		// run
		while (!terminateRequested)
		{
		   // reset if so requested
		   if (resetRequested)
		   {
		      cpu6502.Reset();
		      resetRequested = false;
		   }

			// Run the processor until the 3KHz clock changes
			int cyclesToRun = (int)(clockCyclesPer3KHzHalfWave - (totalClockCycles % clockCyclesPer3KHzHalfWave));
			int newClockCycles = 0;
			while (newClockCycles < cyclesToRun)
			{
			   uint16_t pc = cpu6502.GetPC();

				// pause if we hit a breakpoint
				if (breakpoints[pc] || state == StepState)
				{
					state = Stopped;
					requestedAction = NoAction;

					while (!terminateRequested)
					{
						if (requestedAction == StepAction)
						{
							state = StepState;
							break;
						}
						else if (requestedAction == ResumeAction)
						{
							state = Running;
							break;
						}
						environment->Sleep(50);
					}
				}

				// execute the next instruction
            int clockCyclesThisInstruction = cpu6502.SingleStep();
            newClockCycles += clockCyclesThisInstruction;
            tempestBus.IncrementClockCycleCount(clockCyclesThisInstruction);
			   uint16_t newPC = cpu6502.GetPC();
            if (newPC < 0x9000)
			   {
			      char s[100];
			      sprintf(s, "Bad address %X jumped to from %X", newPC, pc);
			      throw TempestException(s);
			   }
			}

			// update our master counter
			totalClockCycles += newClockCycles;

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
	catch (...)
	{
	   processorStatus = "Tempest runner unknown exception";
	}

	state = Terminated;
}
