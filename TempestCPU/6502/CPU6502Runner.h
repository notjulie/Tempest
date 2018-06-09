
#ifndef CPU6502RUNNER_H
#define CPU6502RUNNER_H

#include <chrono>
#include <functional>
#include <map>
#include "CPU6502.h"

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class CPU6502Runner
{
public:
   // no public constructors allowed, use the static instantiator
   static CPU6502Runner *Create(AbstractBus *bus);
   ~CPU6502Runner(void);

   void     RegisterHook(uint16_t address, std::function<uint32_t()> hook);
   void     Start(void);
   void     SetBreakpoint(uint16_t address, bool set);

   // simple accessors
   CPU6502     *Get6502(void) { return &cpu6502; }
   std::string GetProcessorStatus(void) { return processorStatus; }
   bool        IsStopped(void) { return state == Stopped && requestedAction == NoAction; }
   void			Step(void) { requestedAction = StepAction; }
   void			Resume(void) { requestedAction = ResumeAction; }
   void        Reset(void) { resetRequested = true; }

   // our main thing that we do
   void  SingleStep(void);

private:
   // this can only be instantiated by the static method; since this is
   // a thread class inheriting it is just too dangerous
   CPU6502Runner(AbstractBus *bus);

private:
   void  Break(void);
   void  CheckForResume(void);
   void  DoSingleStep(void);
   void  SynchronizeCPUWithRealTime(void);

private:
   enum State {
      Unstarted,
      Running,
      StepState,
      Stopped
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
   bool     resetRequested = false;
   std::chrono::high_resolution_clock::time_point cpuTime;
   uint8_t	addressFlags[64 * 1024];
   std::map<uint16_t, std::function<uint32_t()> > hooks;
   std::string processorStatus;

   AbstractBus *bus = nullptr;
   CPU6502		cpu6502;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
