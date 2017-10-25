// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is the top level aggregator of the Tempest main board.  It
//    steps the processor and coordinates access to things such as the
//    vector data so that someone outside of this module can display
//    the vectors.
// ====================================================================

#include "stdafx.h"
#include <stdarg.h>
#include <chrono>

#include "6502/CPU6502Exception.h"
#include "SimpleVectorDataInterpreter.h"
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
	for (int i = 0; i < 64 * 1024; ++i)
		addressFlags[i] = 0;
   playerScores[0] = playerScores[1] = 0;
   for (int i = 0; i < HIGH_SCORE_COUNT; ++i)
      highScores[i] = 10101;

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
   Register6502Hooks();
   RegisterVectorHooks();

   // register timers
   tempestBus.StartTimer(1500, [this]() { SynchronizeCPUWithRealTime(); });
}

TempestRunner::~TempestRunner(void)
{
	if (theThread != nullptr)
	{
		terminateRequested = true;
		((std::thread *)theThread)->join();
      delete (std::thread *)theThread;
      theThread = nullptr;
	}
}


void TempestRunner::RegisterVectorHooks(void)
{
   // add a hook that displays the player one score
   vectorInterpreter.RegisterHook(DISPLAY_PLAYER1_SCORE_ADDRESS,
      [this](uint16_t pc) {
      vectorInterpreter.Printf("%6d", playerScores[0]);
      return (uint16_t)(pc + 12);
   }
   );

   // add a hook that displays the player two score
   vectorInterpreter.RegisterHook(DISPLAY_PLAYER2_SCORE_ADDRESS,
      [this](uint16_t pc) {
      vectorInterpreter.Printf("%6d", playerScores[1]);
      return (uint16_t)(pc + 12);
   }
   );

   // add a hook that displays the high score
   vectorInterpreter.RegisterHook(DISPLAY_HIGH_SCORE_ADDRESS,
      [this](uint16_t pc) {
      vectorInterpreter.Printf("%6d", highScores[0]);
      return (uint16_t)(pc + 12);
   }
   );
}

void TempestRunner::Register6502Hooks(void)
{
   // add in our handler for when the game adds points to the player's score
   RegisterHook(INCREASE_PLAYER_SCORE_ROUTINE, [this]() {
      return AddToScore();
   });

   // add in our handler for when the game clears player scores
   RegisterHook(CLEAR_PLAYER_SCORE_ROUTINE, [this]() {
      SetPlayerScore(0, 0);
      SetPlayerScore(1, 0);
      cpu6502.RTS();
      return 30;
   });

   // this is the routine that outputs the score part of a line in the high
   // score table
   RegisterHook(OUTPUT_HIGH_SCORE_ROUTINE, [this]() {
      // x tells us which score we're displaying
      uint8_t x = cpu6502.GetX();
      int highScoreIndex = 7 - x / 3;
      Printf("%7d", highScores[highScoreIndex]);
      cpu6502.JMP(OUTPUT_HIGH_SCORE_ROUTINE_EXIT);
      return 100;
   });

   // this gets called when it's time to sort the high score table and see
   // if player 1 or player 2 belong in it
   RegisterHook(CHECK_HIGH_SCORE_ROUTINE, [this]() {
      return SortHighScores();
   });

   // this gets called after a player has entered his high score
   RegisterHook(CHECK_NEXT_PLAYER_HIGH_SCORE, [this]() {
      if (tempestBus.ReadByte(CURRENT_PLAYER) == 0)
      {
         // set current player
         tempestBus.WriteByte(CURRENT_PLAYER, 1);

         // and skip to the high score entry check
         cpu6502.JMP(HIGH_SCORE_ENTRY);
      }
      else
      {
         // set the game mode to the high score screen
         tempestBus.WriteByte(GAME_MODE, GAME_MODE_SHOW_HIGH_SCORES);

         // skip out of here
         cpu6502.RTS();
      }

      return (uint32_t)10;
   });
}


/// <summary>
/// Synchronizes the CPU with realtime such that the CPU executes instructions
/// at a rate of 1.5MHz, the same as the actual Tempest 6502 CPU.  It does this
/// by doing a thread sleep whenever time measured by CPU cycles gets ahead of
/// system time.
/// </summary>
void TempestRunner::SynchronizeCPUWithRealTime(void)
{
   // we get called every millisecond according to the number of clock cycles
   // executed by the 6502... our job is to align that with real time by pausing
   // to let realtime catch up
   cpuTime += std::chrono::microseconds(1000);

   // figure out how far ahead of realtime the CPU time is
   auto now = std::chrono::high_resolution_clock::now();
   auto cpuAheadTime = std::chrono::duration_cast<std::chrono::microseconds>(cpuTime - now);

   // if we're more than a second off of realtime just forget about it and try to synch
   // up with the current reality
   if (cpuAheadTime.count() < -1000000 || cpuAheadTime.count() > 1000000)
   {
      cpuTime = now;
      return;
   }

   // if the CPU is ahead of realtime (which it usually should be) then
   // sleep to let time catch up
   if (cpuAheadTime.count() > 0)
      std::this_thread::sleep_for(cpuAheadTime);
}


uint32_t TempestRunner::SortHighScores(void)
{
   // figure out how many scores we're dealing with
   int numberOfPlayers = 1 + tempestBus.ReadByte(PLAYER_COUNT);

   // to the 6502 app, a rank of zero means no score
   uint8_t player1Rank = 0;
   uint8_t player2Rank = 0;

   // figure out the rank of player 1
   player1Rank = InsertHighScore(playerScores[0]);

   // and the rank of player 2, if we have a player 2
   if (numberOfPlayers > 1)
   {
      player2Rank = InsertHighScore(playerScores[1]);

      // and of course if player 2 beat player 1 that moves player 1
      // down in the rankings
      if (player2Rank <= player1Rank)
         ++player1Rank;
   }

   // store the ranks
   tempestBus.WriteByte(PLAYER1_RANK, player1Rank);
   tempestBus.WriteByte(PLAYER2_RANK, player2Rank);

   // set current player
   tempestBus.WriteByte(CURRENT_PLAYER, 0);

   // and skip the 6502 implementation
   cpu6502.JMP(HIGH_SCORE_ENTRY);

   // fake some clock cycles
   return 200;
}

uint8_t TempestRunner::InsertHighScore(uint32_t score)
{
   for (uint8_t i = 0; i < HIGH_SCORE_COUNT; ++i)
   {
      if (score > highScores[i])
      {
         // we find were it's supposed to be, so move everything else down
         for (int j = HIGH_SCORE_COUNT - 1; j > i; --j)
            highScores[j] = highScores[j - 1];

         // insert the new score
         highScores[i] = score;

         // and do the same with the initials... we let the 6502 manage those
         // so move some RAM around
         if (i < 8)
         {
            for (int j = 7; j > i; --j)
            {
               for (int n = 0; n < 3; ++n)
               {
                  uint16_t address = (uint16_t)(HIGH_SCORE_INITIALS - 3 * j - n);
                  tempestBus.WriteByte(address, tempestBus.ReadByte((uint16_t)(address + 3)));
               }
            }
            for (int n = 0; n < 3; ++n)
            {
               uint16_t address = (uint16_t)(HIGH_SCORE_INITIALS - 3 * i - n);
               tempestBus.WriteByte(address, 0);
            }
         }

         // and the caller expects a 1-based rank instead of a zero-based index
         return ++i;
      }
   }

   return HIGH_SCORE_COUNT;
}


/// <summary>
/// This is only useful if called from a point in the program where the
/// 6502 program is writing characters to a location pointed to by a
/// zero-page vector.  In such case, it writes characters per the rules
/// of printf, limited of course to the Tempest character set.
/// </summary>
void TempestRunner::Printf(const char *format, ...)
{
   char buffer[256];
   va_list args;
   va_start(args, format);
   vsnprintf(buffer, sizeof(buffer), format, args);

   for (unsigned i = 0; i < sizeof(buffer); ++i)
   {
      if (buffer[i] == 0)
         break;
      Char(buffer[i]);
   }
}

void TempestRunner::Char(char c)
{
   // This is valid only in a particular context where the game is writing out text using
   // a particular subroutine.  In that context, the address in vector RAM to which we
   // write is stored at 0x0074.
   uint16_t targetAddress = (uint16_t)(tempestBus.ReadByte(0x0074) + 256 * tempestBus.ReadByte(0x0075));

   // We want a JSR instruction to the address of the routine for that character, which looks
   // like this
   uint16_t jsrInstruction = (uint16_t)(0xA000 + (vectorInterpreter.GetCharSubroutineAddress(c)/2));

   // store the instruction
   tempestBus.WriteByte(targetAddress++, (uint8_t)jsrInstruction);
   tempestBus.WriteByte(targetAddress++, (uint8_t)(jsrInstruction>>8));

   // update the target address
   tempestBus.WriteByte(0x0074, (uint8_t)targetAddress);
   tempestBus.WriteByte(0x0075, (uint8_t)(targetAddress >> 8));
}

void TempestRunner::RegisterHook(uint16_t address, std::function<uint32_t()> hook)
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
   // open the database
   db.Open(environment->GetDatabasePathName());

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
      cpuTime = std::chrono::high_resolution_clock::now();

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

               std::this_thread::sleep_for(std::chrono::milliseconds(50));
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

         // check the current PC address... this should actually be handled by the bus someday
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

void TempestRunner::GetAllVectors(std::vector<SimpleVector> &vectors)
{
   // NOTE: this typically does not get called on the 6502 thread, so
   // be careful what you do here... everything we call should be safe

   // get the latest vector data
   VectorData  vectorData;
   tempestBus.GetVectorData(vectorData);

   // make a vector generator
   SimpleVectorGenerator vectorGenerator;

   // interpret it
   vectorInterpreter.SetVectorData(vectorData);
   vectorInterpreter.Interpret(&vectorGenerator);

   // return the result
   vectorGenerator.GetAllVectors(vectors);
}

uint32_t TempestRunner::AddToScore(void)
{
   // this does nothing if we're not in the right mode... for now I just know
   // that the high bit of address 0005 should be set, but probably I can learn
   // more about this
   if ((tempestBus.ReadByte(0x0005) & 0x80) == 0)
   {
      cpu6502.RTS();
      return 10;
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

   // get the score we're adding it to and add
   uint8_t playerIndex = tempestBus.ReadByte(0x003D);
   uint32_t score = playerScores[playerIndex];
   uint32_t newScore = score + value;
   SetPlayerScore(playerIndex, newScore);

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
         return 100;
      }
   }

   // exit the subroutine... we just did all it's work for it
   cpu6502.RTS();

   // return the approximate number of clock cycles we think the old routine would have
   // taken... this doesn't have to be at all exact, it just helps try to keep things
   // running the way they always have
   return 100;
}

void TempestRunner::SetPlayerScore(uint8_t playerIndex, uint32_t newScore)
{
   // update our copy of the score
   playerScores[playerIndex] = newScore;

   // store the updated score to Tempest RAM so that Tempest can
   // write it to the screen
   uint8_t playerScoreOffset = (uint8_t)(3 * playerIndex);
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
      CPU6502::ToBCD((uint8_t)((newScore / 10000) % 100))
      );
}
