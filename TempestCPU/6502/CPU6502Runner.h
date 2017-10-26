
#ifndef CPU6502RUNNER_H
#define CPU6502RUNNER_H

#include "CPU6502.h"

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class CPU6502Runner
{
public:
   CPU6502Runner(void);
   virtual ~CPU6502Runner(void);

   void           SetBus(AbstractBus *bus);
   virtual void	Start(void);
   void           RegisterHook(uint16_t address, std::function<uint32_t()> hook);
   void           SetBreakpoint(uint16_t address, bool set);

   // simple accessors
   std::string GetProcessorStatus(void) { return processorStatus; }
   bool        IsStopped(void) { return state == Stopped && requestedAction == NoAction; }
   bool		   IsTerminated(void) { return state == Terminated; }
   void			Step(void) { requestedAction = StepAction; }
   void			Resume(void) { requestedAction = ResumeAction; }
   void        Reset(void) { resetRequested = true; }

   // simple dispatches to the CPU6502 object
   uint8_t  GetAccumulator(void) { return cpu6502.GetA(); }
   uint16_t GetProgramCounter(void) { return cpu6502.GetPC(); }
   uint8_t  GetXRegister(void) { return cpu6502.GetX(); }
   uint8_t  GetYRegister(void) { return cpu6502.GetY(); }
   uint8_t  GetStackPointer(void) { return cpu6502.GetS(); }

protected:
   CPU6502 *Get6502(void) { return &cpu6502; }

private:
   void	RunnerThread(void);
   void  SynchronizeCPUWithRealTime(void);

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

private:
   State    state = Unstarted;
   Action   requestedAction = NoAction;
   bool     terminateRequested = false;
   bool     resetRequested = false;
   std::chrono::high_resolution_clock::time_point cpuTime;
   uint8_t	addressFlags[64 * 1024];
   std::map<uint16_t, std::function<uint32_t()> > hooks;
   std::string processorStatus;

   // this is actually a pointer to a std::thread, but the .NET CLR doesn't allow including
   // <thread> in any file it compiles, so we leave the detail private to the CPP file
   void *theThread = nullptr;

   CPU6502		cpu6502;
   AbstractBus *bus = nullptr;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
