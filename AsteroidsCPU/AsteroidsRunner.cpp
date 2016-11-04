
#include "stdafx.h"

#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestException.h"

#include "AsteroidsRunner.h"

AsteroidsRunner::AsteroidsRunner(AbstractTempestEnvironment *_environment)
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
	theThread = NULL;
	for (int i = 0; i < 64 * 1024; ++i)
		addressFlags[i] = 0;
   resetRequested = false;
   pointsPerBonusLife = 10000;
   playerScores[0] = playerScores[1] = 0;

   // register commands
   environment->RegisterCommand(
      "?pc",
      [this](const CommandLine &) {
            std::ostringstream s;
            s << std::setfill('0') << std::hex << std::uppercase;
            s << "PC: " << std::setw(4) << cpu6502.GetPC();
            return s.str();
         }
      );
}

AsteroidsRunner::~AsteroidsRunner(void)
{
	if (theThread != NULL)
	{
		terminateRequested = true;
		theThread->join();
		delete theThread, theThread = NULL;
	}
}


void AsteroidsRunner::RegisterHook(uint16_t address, std::function<uint32_t()> hook)
{
   hooks[address] = hook;
   addressFlags[address] |= HOOK;
}

void AsteroidsRunner::SetDemoMode(void)
{
   // tell the bus to set the proper DIP switches
   tempestBus.SetDemoMode();

   // force a reset
   resetRequested = true;
}

void AsteroidsRunner::Start(void)
{
	// create the thread is all
	theThread = new std::thread(
      [this]() { RunnerThread(); }
      );
}


void AsteroidsRunner::RunnerThread(void)
{
	try
	{
		// set our state
		state = Running;

		// reset the CPU and the realtime clock
		cpu6502.Reset();

		// synchronize our clock with the real world
		environment->Reset();

		// run
		while (!terminateRequested)
		{
		   // reset if so requested
		   if (resetRequested)
		   {
		      cpu6502.Reset();
		      resetRequested = false;
		   }

			uint16_t pc = cpu6502.GetPC();
         uint8_t flags = addressFlags[pc];

			// pause if we hit a breakpoint
			if ((flags&BREAKPOINT) || state == StepState)
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

         // execute a hook if we have one at this address
         if (flags & HOOK)
         {
            tempestBus.IncrementClockCycleCount(hooks[pc]());

            // if the program counter has changed we should skip to the top of the loop
            // in case it brought us to a break point
            if (cpu6502.GetPC() != pc)
               continue;
         }

			// execute the next instruction
         if (tempestBus.IsPaused())
         {
            tempestBus.ClearWatchdog();
            tempestBus.IncrementClockCycleCount(10);
         }
         else
         {
            uint32_t clockCyclesThisInstruction = cpu6502.SingleStep();
            tempestBus.IncrementClockCycleCount(clockCyclesThisInstruction);
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

void AsteroidsRunner::SetBreakpoint(uint16_t address, bool set)
{
   if (set)
      addressFlags[address] |= BREAKPOINT;
   else
      addressFlags[address] &= ~BREAKPOINT;
}

