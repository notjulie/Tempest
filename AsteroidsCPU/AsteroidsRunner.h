
#ifndef ASTEROIDSRUNNER_H
#define ASTEROIDSRUNNER_H

#include <chrono>
#include "TempestCPU/6502/CPU6502.h"
#include "AsteroidsBus.h"

class AbstractTempestEnvironment;
class AsteroidsVSM;

namespace std {
   class thread;
};

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class AsteroidsRunner
{
private:
	enum State {
		Unstarted,
		Running,
		StepState,
		Stopped,
		Terminated
	};

	enum Action {
		NoAction,
		ResumeAction,
		StepAction
	};

   enum AddressFlags {
      BREAKPOINT = 1,
      HOOK = 2
   };

public:
   AsteroidsRunner(AbstractTempestEnvironment *environment);
   virtual ~AsteroidsRunner(void);
   AsteroidsRunner(const AsteroidsRunner &tr) = delete;
   AsteroidsRunner &operator=(const AsteroidsRunner &tr) = delete;

	void	Start(void);

	// simple accessors
	std::string GetProcessorStatus(void) { return processorStatus; }
	bool        IsStopped(void) { return state == Stopped && requestedAction==NoAction; }
	bool		   IsTerminated(void) { return state == Terminated; }
   void        SetBreakpoint(uint16_t address, bool set);
	void			Step(void) { requestedAction = StepAction; }
	void			Resume(void) { requestedAction = ResumeAction; }

	// simple dispatches to the CPU6502 object
	uint8_t  GetAccumulator(void) { return cpu6502.GetA(); }
	uint16_t GetProgramCounter(void) { return cpu6502.GetPC(); }
	uint8_t  GetXRegister(void) { return cpu6502.GetX(); }
	uint8_t  GetYRegister(void) { return cpu6502.GetY(); }
	uint8_t  GetStackPointer(void) { return cpu6502.GetS(); }

	// simple dispatches to the AsteroidsBus object
   bool     HaveVectorData(void) { return asteroidsBus.IsVectorGo(); }
   uint64_t GetTotalClockCycles(void) { return asteroidsBus.GetTotalClockCycles(); }
   void     GetVectorData(AsteroidsVSM &vectorData) { asteroidsBus.GetVectorData(vectorData); }
   void     SetTempestIO(AbstractTempestSoundIO *tempestSoundIO) { asteroidsBus.SetTempestIO(tempestSoundIO); }

private:
   void  RegisterHook(uint16_t address, std::function<uint32_t()> hook);
	void	RunnerThread(void);

private:
	AbstractTempestEnvironment *environment;

   std::chrono::high_resolution_clock::time_point referenceTime;
   std::chrono::microseconds cpuAheadTime;
   
   bool     terminateRequested;
	bool     resetRequested;
	State    state;
	Action   requestedAction;
   int pointsPerBonusLife;

	AsteroidsBus	asteroidsBus;
	CPU6502		cpu6502;

	std::string processorStatus;
	std::thread *theThread;
	uint8_t	addressFlags[64 * 1024];
   std::map<uint16_t, std::function<uint32_t()> > hooks;
   uint32_t playerScores[2];
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
