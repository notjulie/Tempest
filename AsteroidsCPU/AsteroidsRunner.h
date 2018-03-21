
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

class AsteroidsRunner
{
public:
   AsteroidsRunner(AbstractTempestEnvironment *environment);
   ~AsteroidsRunner(void);
   AsteroidsRunner(const AsteroidsRunner &tr) = delete;
   AsteroidsRunner &operator=(const AsteroidsRunner &tr) = delete;

	// simple dispatches to the AsteroidsBus object
   bool     HaveVectorData(void) { return asteroidsBus.IsVectorGo(); }
   uint64_t GetTotalClockCycles(void) { return asteroidsBus.GetTotalClockCycles(); }
   void     GetVectorData(AsteroidsVectorInterpreter &vectorData) { asteroidsBus.GetVectorData(vectorData); }
   void     SetTempestIO(AbstractTempestSoundIO *tempestSoundIO) { asteroidsBus.SetTempestIO(tempestSoundIO); }

   // simple dispatches to the runner
   uint8_t  GetAccumulator(void) { return cpuRunner->Get6502()->GetA(); }
   int      GetProgramCounter(void) { return cpuRunner->Get6502()->GetPC(); }
   uint8_t  GetXRegister(void) { return cpuRunner->Get6502()->GetX(); }
   uint8_t  GetYRegister(void) { return cpuRunner->Get6502()->GetY(); }
   void     Step(void) { cpuRunner->Step(); }
   void     Resume(void) { cpuRunner->Resume(); }
   bool     IsStopped(void) const { return cpuRunner->IsStopped(); }
   void     SetBreakpoint(uint16_t address, bool set) { cpuRunner->SetBreakpoint(address, set); }
   void     Start(void) { cpuRunner->Start(); }
   bool     IsTerminated(void) const { return cpuRunner->IsTerminated(); }
   std::string GetProcessorStatus() const { return cpuRunner->GetProcessorStatus(); }

private:
   CPU6502Runner *cpuRunner = nullptr;
	AbstractTempestEnvironment *environment;
	AsteroidsBus	asteroidsBus;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
