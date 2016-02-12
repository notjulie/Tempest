
#ifndef TEMPESTRUNNER_H
#define TEMPESTRUNNER_H

#include "6502/CPU6502.h"
#include "TempestBus.h"

class AbstractTempestEnvironment;
class AbstractTempestVectorIO;
class AbstractThread;


#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

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

public:
	TempestRunner(AbstractTempestEnvironment *environment);
	virtual ~TempestRunner(void);

	void	Start(void);

	// simple accessors
	std::string GetProcessorStatus(void) { return processorStatus; }
	bool        IsStopped(void) { return state == Stopped && requestedAction==NoAction; }
	bool		   IsTerminated(void) { return state == Terminated; }
	void        SetBreakpoint(uint16_t address, bool set) { breakpoints[address] = set; }
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
   void     SetTempestIO(AbstractTempestSoundIO *tempestSoundIO, AbstractTempestVectorIO *tempestVectorIO) { tempestBus.SetTempestIO(tempestSoundIO, tempestVectorIO); }

private:
	void	RunnerThread(void);

private:
	static void RunnerThreadEntry(void *pThis) { ((TempestRunner*)pThis)->RunnerThread(); }

private:
	// forbidden
	TempestRunner(const TempestRunner &tr);
	TempestRunner &operator=(const TempestRunner &tr);

private:
	AbstractTempestEnvironment *environment;

	bool     terminateRequested;
	bool     resetRequested;
	State    state;
	Action   requestedAction;

	TempestBus	tempestBus;
	CPU6502		cpu6502;

	std::string processorStatus;
	AbstractThread *theThread;
	bool	breakpoints[64 * 1024];
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
