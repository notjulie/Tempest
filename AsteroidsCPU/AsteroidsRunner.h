
#ifndef ASTEROIDSRUNNER_H
#define ASTEROIDSRUNNER_H

#include <chrono>
#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Runner.h"
#include "AsteroidsBus.h"

class AbstractTempestEnvironment;
class AsteroidsVSM;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class AsteroidsRunner : public CPU6502Runner
{
public:
   AsteroidsRunner(AbstractTempestEnvironment *environment);
   virtual ~AsteroidsRunner(void);
   AsteroidsRunner(const AsteroidsRunner &tr) = delete;
   AsteroidsRunner &operator=(const AsteroidsRunner &tr) = delete;

	// simple dispatches to the AsteroidsBus object
   bool     HaveVectorData(void) { return asteroidsBus.IsVectorGo(); }
   uint64_t GetTotalClockCycles(void) { return asteroidsBus.GetTotalClockCycles(); }
   void     GetVectorData(AsteroidsVSM &vectorData) { asteroidsBus.GetVectorData(vectorData); }
   void     SetTempestIO(AbstractTempestSoundIO *tempestSoundIO) { asteroidsBus.SetTempestIO(tempestSoundIO); }

private:
	AbstractTempestEnvironment *environment;
	AsteroidsBus	asteroidsBus;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
