
#ifndef TEMPESTRUNNER_H
#define TEMPESTRUNNER_H

#include <chrono>
#include <string>

#include "6502/CPU6502.h"
#include "6502/CPU6502Runner.h"
#include "TempestBus.h"
#include "TempestDB.h"
#include "VectorDataInterpreter.h"

class AbstractTempestEnvironment;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

struct SimpleVector;

const uint8_t HIGH_SCORE_COUNT = 100;

class TempestRunner
{
public:
	TempestRunner(AbstractTempestEnvironment *environment);
	virtual ~TempestRunner(void);

   void	Start(void);
   
	// simple dispatches to the TempestBus object
   uint64_t GetTotalClockCycles(void) { return tempestBus.GetTotalClockCycles(); }
   bool     IsInAttractMode(void) { return tempestBus.ReadByte(GAME_INPUT_MODE) == GAME_INPUT_MODE_ATTRACT; }
   void     SetDemoMode(void);
   void     SetTempestIO(AbstractTempestSoundIO *tempestSoundIO) { tempestBus.SetTempestIO(tempestSoundIO); }

   // function for getting display vector data
   void GetAllVectors(std::vector<SimpleVector> &vectors);

   // simple dispatches to the runner
   uint8_t  GetAccumulator(void) { return cpuRunner->Get6502()->GetA(); }
   int      GetProgramCounter(void) { return cpuRunner->Get6502()->GetPC(); }
   uint8_t  GetXRegister(void) { return cpuRunner->Get6502()->GetX(); }
   uint8_t  GetYRegister(void) { return cpuRunner->Get6502()->GetY(); }
   void     Step(void) { cpuRunner->Step(); }
   void     Resume(void) { cpuRunner->Resume(); }
   bool     IsStopped(void) const { return cpuRunner->IsStopped(); }
   void     SetBreakpoint(uint16_t address, bool set) { cpuRunner->SetBreakpoint(address, set); }
   bool     IsTerminated(void) const { return cpuRunner->IsTerminated(); }
   std::string GetProcessorStatus() const { return cpuRunner->GetProcessorStatus(); }

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
	// forbidden
	TempestRunner(const TempestRunner &tr) = delete;
	TempestRunner &operator=(const TempestRunner &tr) = delete;

private:
	AbstractTempestEnvironment *environment;
   CPU6502Runner *cpuRunner = nullptr;

   int pointsPerBonusLife = 10000;

   TempestDB   db;
	TempestBus	tempestBus;
   VectorDataInterpreter vectorInterpreter;

   uint32_t playerScores[2];
   uint32_t highScores[HIGH_SCORE_COUNT];
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
