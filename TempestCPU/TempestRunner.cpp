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


VectorGameRunner::VectorGameRunner(VectorGame *_game)
{
   // copy parameters
   game = _game;

   // create objects
   cpuRunner = CPU6502Runner::Create(game->GetBus());
}

VectorGameRunner::~VectorGameRunner(void)
{
   // stop the runner thread
   delete cpuRunner;
   cpuRunner = nullptr;
}


void VectorGameRunner::Start(void)
{
   // tell the game to start
   game->Start();

   // tell it to register its hooks
   game->Register6502Hooks(cpuRunner);

   // start running the CPU
   cpuRunner->Start();
}


TempestGame::TempestGame(AbstractTempestEnvironment *_environment)
	:
		tempestBus(_environment)
{
   // save parameters
   environment = _environment;

   // clear
   playerScores[0] = playerScores[1] = 0;
   for (int i = 0; i < HIGH_SCORE_COUNT; ++i)
      highScores[i] = 10101;

   // register hooks
   RegisterVectorHooks();
}

TempestGame::~TempestGame(void)
{
}


void TempestGame::Start(void)
{
   // open the database
   db.Open(environment->GetDatabasePathName());
}


void TempestGame::RegisterVectorHooks(void)
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

void TempestGame::Register6502Hooks(CPU6502Runner *cpuRunner)
{
   // add in our handler for when the game adds points to the player's score
   cpuRunner->RegisterHook(INCREASE_PLAYER_SCORE_ROUTINE, [this,cpuRunner]() {
      return AddToScore(cpuRunner);
   });

   // add in our handler for when the game clears player scores
   cpuRunner->RegisterHook(CLEAR_PLAYER_SCORE_ROUTINE, [this,cpuRunner]() {
      SetPlayerScore(0, 0);
      SetPlayerScore(1, 0);
      cpuRunner->Get6502()->RTS();
      return 30;
   });

   // this is the routine that outputs the score part of a line in the high
   // score table
   cpuRunner->RegisterHook(OUTPUT_HIGH_SCORE_ROUTINE, [this,cpuRunner]() {
      // x tells us which score we're displaying
      uint8_t x = cpuRunner->Get6502()->GetX();
      int highScoreIndex = 7 - x / 3;
      Printf("%7d", highScores[highScoreIndex]);
      cpuRunner->Get6502()->JMP(OUTPUT_HIGH_SCORE_ROUTINE_EXIT);
      return 100;
   });

   // this gets called when it's time to sort the high score table and see
   // if player 1 or player 2 belong in it
   cpuRunner->RegisterHook(CHECK_HIGH_SCORE_ROUTINE, [this,cpuRunner]() {
      return SortHighScores(cpuRunner);
   });

   // this gets called after a player has entered his high score
   cpuRunner->RegisterHook(CHECK_NEXT_PLAYER_HIGH_SCORE, [this,cpuRunner]() {
      if (tempestBus.ReadByte(CURRENT_PLAYER) == 0)
      {
         // set current player
         tempestBus.WriteByte(CURRENT_PLAYER, 1);

         // and skip to the high score entry check
         cpuRunner->Get6502()->JMP(HIGH_SCORE_ENTRY);
      }
      else
      {
         // set the game mode to the high score screen
         tempestBus.WriteByte(GAME_MODE, GAME_MODE_SHOW_HIGH_SCORES);

         // skip out of here
         cpuRunner->Get6502()->RTS();
      }

      return (uint32_t)10;
   });
}


uint32_t TempestGame::SortHighScores(CPU6502Runner *cpuRunner)
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
   cpuRunner->Get6502()->JMP(HIGH_SCORE_ENTRY);

   // fake some clock cycles
   return 200;
}

uint8_t TempestGame::InsertHighScore(uint32_t score)
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
void TempestGame::Printf(const char *format, ...)
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

void TempestGame::Char(char c)
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

void TempestGame::GetAllVectors(std::vector<SimpleVector> &vectors)
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

uint32_t TempestGame::AddToScore(CPU6502Runner *cpuRunner)
{
   // this does nothing if we're not in the right mode... for now I just know
   // that the high bit of address 0005 should be set, but probably I can learn
   // more about this
   if ((tempestBus.ReadByte(0x0005) & 0x80) == 0)
   {
      cpuRunner->Get6502()->RTS();
      return 10;
   }

   // get the value we are adding from either [29][2A][2B] or a lookup table based on X
   int value = 0;
   switch (cpuRunner->Get6502()->GetX())
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
         cpuRunner->Get6502()->JMP(0xCCB9);
         return 100;
      }
   }

   // exit the subroutine... we just did all it's work for it
   cpuRunner->Get6502()->RTS();

   // return the approximate number of clock cycles we think the old routine would have
   // taken... this doesn't have to be at all exact, it just helps try to keep things
   // running the way they always have
   return 100;
}

void TempestGame::SetPlayerScore(uint8_t playerIndex, uint32_t newScore)
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
