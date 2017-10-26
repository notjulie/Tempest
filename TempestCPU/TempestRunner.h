
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

class TempestRunner : public CPU6502Runner
{
public:
	TempestRunner(AbstractTempestEnvironment *environment);
	virtual ~TempestRunner(void);

   virtual void	Start(void);
   
	// simple dispatches to the TempestBus object
   uint64_t GetTotalClockCycles(void) { return tempestBus.GetTotalClockCycles(); }
   void     SetDemoMode(void);
   void     SetTempestIO(AbstractTempestSoundIO *tempestSoundIO) { tempestBus.SetTempestIO(tempestSoundIO); }

   // function for getting display vector data
   void GetAllVectors(std::vector<SimpleVector> &vectors);

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
