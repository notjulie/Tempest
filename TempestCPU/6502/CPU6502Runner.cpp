// ====================================================================
// Vector game emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is a wrapper around an emulated 6502-based system.  When
//    our SingleStep() method is called, we check to see if we have
//    any special actions to do and determine whether or not we want
//    to pass the call directly to the 6502's SingleStep function.
//    As such we can add in hooks, implement debug breakpoints, etc.
// ====================================================================


#include "TempestCPU.Headers.h"
#include "AbstractBus.h"
#include "CPU6502Runner.h"

/// <summary>
/// The period between checks to see how far ahead of the realtime clock the 6502 has gotten;
/// this actually taxes the CPU a bit if we check too often
/// </summary>
static const int CPU_SYNCHRONIZE_PERIOD_MS = 4;


/// <summary>
/// Constructor
/// </summary>
CPU6502Runner::CPU6502Runner(void)
{
   // clear
   for (int i = 0; i < 64 * 1024; ++i)
      addressFlags[i] = 0;
}

/// <summary>
/// Destructor
/// </summary>
CPU6502Runner::~CPU6502Runner(void)
{
   delete cpu6502; cpu6502 = nullptr;
}


/// <summary>
/// Registers a function to be called when the 6502's program counter hits a
/// certain address... as such we can insert C++ hooks into the 6502 code
/// </summary>
void CPU6502Runner::RegisterHook(uint16_t address, std::function<uint32_t()> hook)
{
   hooks[address] = hook;
   addressFlags[address] |= HOOK;
}


/// <summary>
/// Sets or clears a debug breakpoint
/// </summary>
void CPU6502Runner::SetBreakpoint(uint16_t address, bool set)
{
   if (set)
      addressFlags[address] |= BREAKPOINT;
   else
      addressFlags[address] &= ~BREAKPOINT;
}


/// <summary>
/// Sets our state in response to hitting a breakpoint
/// </summary>
void CPU6502Runner::Break(void)
{
   state = Stopped;
   requestedAction = NoAction;
}


/// <summary>
/// Single steps ourself, taking our own state and hooks into account
/// </summary>
void CPU6502Runner::SingleStep(void)
{
   switch (state)
   {
   case Unstarted:
      // create our CPU
      cpu6502 = new CPU6502(bus);

      // reset the CPU and the realtime clock
      cpu6502->Reset();
      cpuTime = std::chrono::high_resolution_clock::now();

      // register our timer that synchronizes the CPU clock with real time
      bus->StartTimer(1500 * CPU_SYNCHRONIZE_PERIOD_MS, [this]() { SynchronizeCPUWithRealTime(); });

      // start
      state = Running;
      break;

   case StepState:
      Break();
      break;

   case Stopped:
      CheckForResume();
      break;

   case Running:
      DoSingleStep();
      break;
   }
}


/// <summary>
/// Takes action according to the current 6502 program counter
/// </summary>
void CPU6502Runner::DoSingleStep(void)
{
   // reset if so requested
   if (resetRequested)
   {
      cpu6502->Reset();
      resetRequested = false;
   }

   uint16_t pc = cpu6502->GetPC();
   uint8_t flags = addressFlags[pc];

   // pause if we hit a breakpoint
   if (flags & BREAKPOINT)
   {
      Break();
      return;
   }

   // execute a hook if we have one at this address
   if (flags & HOOK)
   {
      bus->IncrementClockCycleCount(hooks[pc]());

      // if the program counter has changed we should skip to the top of the loop
      // in case it brought us to a break point
      if (cpu6502->GetPC() != pc)
         return;
   }

   // execute the next instruction
   uint32_t clockCyclesThisInstruction = cpu6502->SingleStep();
   bus->IncrementClockCycleCount(clockCyclesThisInstruction);
}


/// <summary>
/// Checks to see if we should resume from a breakpoint
/// </summary>
void CPU6502Runner::CheckForResume(void)
{
   if (requestedAction == StepAction)
   {
      state = StepState;
      return;
   }
   else if (requestedAction == ResumeAction)
   {
      state = Running;
      return;
   }

   std::this_thread::sleep_for(std::chrono::milliseconds(50));
}


/// <summary>
/// Synchronizes the CPU with realtime such that the CPU executes instructions
/// at a rate of 1.5MHz, the same as the actual Tempest 6502 CPU.  It does this
/// by doing a thread sleep whenever time measured by CPU cycles gets ahead of
/// system time.
/// </summary>
void CPU6502Runner::SynchronizeCPUWithRealTime(void)
{
   // we get called every CPU_SYNCHRONIZE_PERIOD_MS milliseconds according to
   // the number of clock cycles executed by the 6502... our job is to align
   // that with real time by pausing to let realtime catch up
   cpuTime += std::chrono::microseconds(CPU_SYNCHRONIZE_PERIOD_MS * 1000);

   // figure out how far ahead of realtime the CPU time is
   auto now = std::chrono::high_resolution_clock::now();
   auto cpuAheadTime = std::chrono::duration_cast<std::chrono::microseconds>(cpuTime - now);

   // if we're more than a second off of realtime just forget about it and try to synch
   // up with the current reality
   if (cpuAheadTime.count() < -1000000 || cpuAheadTime.count() > 1000000)
   {
      cpuTime = now;
      return;
   }

   // if the CPU is ahead of realtime (which it usually should be) then
   // sleep to let time catch up
   if (cpuAheadTime.count() >= CPU_SYNCHRONIZE_PERIOD_MS * 1000)
      std::this_thread::sleep_for(cpuAheadTime);
}


