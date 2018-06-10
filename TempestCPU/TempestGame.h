
#ifndef TEMPESTRUNNER_H
#define TEMPESTRUNNER_H

#include <chrono>
#include <string>

#include "6502/CPU6502.h"
#include "6502/CPU6502Runner.h"
#include "TempestBus.h"
#include "TempestDB.h"
#include "VectorDataInterpreter.h"
#include "VectorGame.h"

class AbstractTempestEnvironment;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

struct SimpleVector;

const uint8_t HIGH_SCORE_COUNT = 100;

class TempestGame : public VectorGame
{
public:
   TempestGame(AbstractTempestEnvironment *environment);
   TempestGame(const TempestGame &) = delete;
   TempestGame& operator=(const TempestGame &) = delete;
   virtual ~TempestGame(void);

   void	Start(void);
   
	// simple dispatches to the TempestBus object
   uint64_t GetTotalClockCycles(void) { return tempestBus.GetTotalClockCycles(); }
   bool     IsInAttractMode(void) { return tempestBus.ReadByte(GAME_INPUT_MODE) == GAME_INPUT_MODE_ATTRACT; }

   // base class overrides
   virtual void GetAllVectors(std::vector<SimpleVector> &vectors);
   virtual void SetControlPanel(AbstractArcadeGameControlPanelReader *controlPanel) { tempestBus.SetControlPanel(controlPanel); }
   virtual void SetSoundOutput(AbstractTempestSoundOutput *soundOutput) { tempestBus.SetSoundOutput(soundOutput); }
   virtual CPU6502Runner *GetCPURunner(void) { return &cpuRunner; }
   virtual void SingleStep(void) { cpuRunner.SingleStep(); }

private:
   void  Register6502Hooks(void);
   void  RegisterVectorHooks(void);

private:
   // game modifications and hooks
   uint32_t  AddToScore(void);
   uint8_t   InsertHighScore(uint32_t score);
   void      SetPlayerScore(uint8_t playerIndex, uint32_t score);
   uint32_t  SortHighScores(void);
   void      Printf(const char *format, ...);
   void      Char(char c);

private:
   AbstractTempestEnvironment * environment;
   int pointsPerBonusLife = 10000;

   TempestDB   db;
	TempestBus	tempestBus;
   VectorDataInterpreter vectorInterpreter;
   CPU6502Runner cpuRunner;

   uint32_t playerScores[2];
   uint32_t highScores[HIGH_SCORE_COUNT];
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
