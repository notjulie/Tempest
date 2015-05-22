
#ifndef TEMPESTRUNNER_H
#define TEMPESTRUNNER_H

#include "6502/CPU6502.h"
#include "TempestBus.h"

namespace std {
	class thread;
};
class AbstractTempestEnvironment;
struct SoundChannelStatus;


#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

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
	uint64_t    GetIRQCount(void) { return irqCount; }
	std::string GetProcessorStatus(void) { return processorStatus; }
	uint64_t    GetTotalClockCycles(void) { return totalClockCycles; }
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

	// simple dispatches to the TempestBus object
	void LoadROM(const uint8_t *rom, int length, uint16_t address) { tempestBus.LoadROM(rom, length, address); }
	void LoadMathBoxROM(const uint8_t *rom, int length, char slot) { tempestBus.LoadMathBoxROM(rom, length, slot); }
	void MoveWheel(int delta) { tempestBus.MoveWheel(delta); }
	void SetButtonState(ButtonID button, bool pressed) { tempestBus.SetButtonState(button, pressed); }
	void SetTempestIO(AbstractTempestIO *tempestIO) { tempestBus.SetTempestIO(tempestIO); }

private:
	void	RunnerThread(void);

private:
	static void RunnerThreadEntry(TempestRunner *pThis) { pThis->RunnerThread(); }

private:
	// forbidden
	TempestRunner(const TempestRunner &tr);
	TempestRunner &operator=(const TempestRunner &tr);

private:
	AbstractTempestEnvironment *environment;

	bool     terminateRequested;
	State    state;
	Action   requestedAction;
	uint64_t irqCount;
	uint64_t totalClockCycles;

	CPU6502		cpu6502;
	TempestBus	tempestBus;

	std::string processorStatus;
	std::thread *theThread;
	bool	breakpoints[64 * 1024];
};

#pragma warning(pop)

#endif