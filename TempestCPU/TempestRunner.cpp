
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
	theThread = NULL;
	for (int i = 0; i < 64 * 1024; ++i)
		addressFlags[i] = 0;
   resetRequested = false;
   pointsPerBonusLife = 20000;

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

   // register hooks
   RegisterHook(0xCA6C, [this]() { AddToScore(); });
}

TempestRunner::~TempestRunner(void)
{
	if (theThread != NULL)
	{
		terminateRequested = true;
		theThread->join();
		delete theThread, theThread = NULL;
	}
}


void TempestRunner::RegisterHook(uint16_t address, std::function<void()> hook)
{
   hooks[address] = hook;
   addressFlags[address] |= HOOK;
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
	theThread = new std::thread(
      [this]() { RunnerThread(); }
      );
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
            hooks[pc]();

            // if the program counter has changed we should skip to the top of the loop
            // in case it brought us to a break point
            if (cpu6502.GetPC() != pc)
               continue;
         }

			// execute the next instruction
         int clockCyclesThisInstruction = cpu6502.SingleStep();
         tempestBus.IncrementClockCycleCount(clockCyclesThisInstruction);
			uint16_t newPC = cpu6502.GetPC();
         if (newPC < 0x9000)
			{
			   char s[100];
			   sprintf(s, "Bad address %X jumped to from %X", newPC, pc);
			   throw TempestException(s);
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

void TempestRunner::SetBreakpoint(uint16_t address, bool set)
{
   if (set)
      addressFlags[address] |= BREAKPOINT;
   else
      addressFlags[address] &= ~BREAKPOINT;
}

void TempestRunner::AddToScore(void)
{
   // this does nothing if we're not in the right mode... for now I just know
   // that the high bit of address 0005 should be set, but probably I can learn
   // more about this
   if ((tempestBus.ReadByte(0x0005) & 0x80) == 0)
   {
      cpu6502.RTS();
      return;
   }

   // get the value we are adding from either [29][2A][2B] or a lookup table based on X

   int value = 0;
   switch (cpu6502.GetX())
   {
   case 0: value = 0; break;
   case 1: value = 150; break;
   case 2: value = 200; break;
   case 3: value = 100; break;
   case 4: value = 50; break;
   case 5: value = 250; break;
   case 6: value = 500; break;
   case 7: value = 750; break;
   default:
      value = CPU6502::FromBCD(tempestBus.ReadByte(0x29)) +
         100 * CPU6502::FromBCD(tempestBus.ReadByte(0x2A)) +
         10000 * CPU6502::FromBCD(tempestBus.ReadByte(0x2B))
         ;
      break;
   }

   // get the score we're adding it to... either [40][41][42] for
   // player 1 or [43][44][45] for player 2
   uint8_t playerIndex = tempestBus.ReadByte(0x003D);
   uint8_t playerScoreOffset = (uint8_t)(3 * playerIndex);
   int score =
      CPU6502::FromBCD(tempestBus.ReadByte((uint16_t)(0x40 + playerScoreOffset))) +
      100 * CPU6502::FromBCD(tempestBus.ReadByte((uint16_t)(0x41 + playerScoreOffset))) +
      10000 * CPU6502::FromBCD(tempestBus.ReadByte((uint16_t)(0x42 + playerScoreOffset)))
      ;

   // add and store
   int newScore = score + value;
   tempestBus.WriteByte(
      (uint16_t)(0x40 + playerScoreOffset),
      CPU6502::ToBCD((uint8_t)(newScore % 100))
      );
   tempestBus.WriteByte(
      (uint16_t)(0x41 + playerScoreOffset),
      CPU6502::ToBCD((uint8_t)((newScore / 100) % 100))
      );
   tempestBus.WriteByte(
      (uint16_t)(0x42 + playerScoreOffset),
      CPU6502::ToBCD((uint8_t)(newScore / 10000))
      );

   // add a life if necessary
   uint8_t lives = tempestBus.ReadByte((uint16_t)(0x0048 + playerIndex));
   if (pointsPerBonusLife > 0 && lives<6)
   {
      if (newScore / pointsPerBonusLife > score / pointsPerBonusLife)
      {
         tempestBus.WriteByte((uint16_t)(0x0048 + playerIndex), (uint8_t)(lives + 1));

         // I thiiiink this starts a counter for how long the fanfare for the new life lasts
         tempestBus.WriteByte(0x0124, 0x20);

         // jump to the routine that starts the fanfare
         cpu6502.JMP(0xCCB9);
         return;
      }
   }

   // exit the subroutine... we just did all it's work for it
   cpu6502.RTS();
}
