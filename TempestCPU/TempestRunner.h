
#ifndef TEMPESTRUNNER_H
#define TEMPESTRUNNER_H

#include <string>
#include <thread>

#include "6502/CPU6502.h"
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
	TempestRunner(AbstractTempestEnvironment *environment);
	virtual ~TempestRunner(void);

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

	// simple dispatches to the TempestBus object
   uint64_t GetTotalClockCycles(void) { return tempestBus.GetTotalClockCycles(); }
   void     SetDemoMode(void);
   void     SetTempestIO(AbstractTempestSoundIO *tempestSoundIO) { tempestBus.SetTempestIO(tempestSoundIO); }

   // function for getting display vector data
   void GetAllVectors(std::vector<SimpleVector> &vectors);

private:
   void  RegisterHook(uint16_t address, std::function<uint32_t()> hook);
   void  Register6502Hooks(void);
   void  RegisterVectorHooks(void);
   void	RunnerThread(void);

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

	bool     terminateRequested;
	bool     resetRequested;
	State    state;
	Action   requestedAction;
   int pointsPerBonusLife;

   TempestDB   db;
	TempestBus	tempestBus;
	CPU6502		cpu6502;
   VectorDataInterpreter vectorInterpreter;

	std::string processorStatus;
	std::thread *theThread;
	uint8_t	addressFlags[64 * 1024];
   std::map<uint16_t, std::function<uint32_t()> > hooks;
   uint32_t playerScores[2];
   uint32_t highScores[HIGH_SCORE_COUNT];
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
