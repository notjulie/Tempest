
#include "stdafx.h"

#include "AbstractBus.h"
#include "CPU6502Exception.h"
#include "TempestException.h"

#include "CPU6502Runner.h"


CPU6502Runner::CPU6502Runner(AbstractBus *_bus)
   :
      bus(_bus),
      cpu6502(_bus)
{
   // clear
   for (int i = 0; i < 64 * 1024; ++i)
      addressFlags[i] = 0;
}

CPU6502Runner::~CPU6502Runner(void)
{
   if (theThread != nullptr)
   {
      terminateRequested = true;
      ((std::thread *)theThread)->join();
      delete (std::thread *)theThread;
      theThread = nullptr;
   }
}


CPU6502Runner *CPU6502Runner::Create(AbstractBus *bus)
{
   return new CPU6502Runner(bus);
}

void CPU6502Runner::RegisterHook(uint16_t address, std::function<uint32_t()> hook)
{
   hooks[address] = hook;
   addressFlags[address] |= HOOK;
}


void CPU6502Runner::SetBreakpoint(uint16_t address, bool set)
{
   if (set)
      addressFlags[address] |= BREAKPOINT;
   else
      addressFlags[address] &= ~BREAKPOINT;
}


void CPU6502Runner::Start(void)
{
   // register our timer that synchronizes the CPU clock with real time
   bus->StartTimer(1500, [this]() { SynchronizeCPUWithRealTime(); });

   // create the thread is all
   theThread = new std::thread(
      [this]() { RunnerThread(); }
   );
}


void CPU6502Runner::RunnerThread(void)
{
   try
   {
      // set our state
      state = Running;

      // reset the CPU and the realtime clock
      cpu6502.Reset();
      cpuTime = std::chrono::high_resolution_clock::now();

      // run
      while (!terminateRequested)
      {
         // reset if so requested
         if (resetRequested)
         {
            cpu6502.Reset();
            resetRequested = false;
         }

         uint16_t pc = cpu6502.GetPC();
         uint8_t flags = addressFlags[pc];

         // pause if we hit a breakpoint
         if ((flags&BREAKPOINT) || state == StepState)
         {
            state = Stopped;
            requestedAction = NoAction;

            while (!terminateRequested)
            {
               if (requestedAction == StepAction)
               {
                  state = StepState;
                  break;
               }
               else if (requestedAction == ResumeAction)
               {
                  state = Running;
                  break;
               }

               std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
         }

         // execute a hook if we have one at this address
         if (flags & HOOK)
         {
            bus->IncrementClockCycleCount(hooks[pc]());

            // if the program counter has changed we should skip to the top of the loop
            // in case it brought us to a break point
            if (cpu6502.GetPC() != pc)
               continue;
         }

         // execute the next instruction
         if (!bus->IsPaused())
         {
            uint32_t clockCyclesThisInstruction = cpu6502.SingleStep();
            bus->IncrementClockCycleCount(clockCyclesThisInstruction);
         }

         // check the current PC address... this should actually be handled by the bus someday
         uint16_t newPC = cpu6502.GetPC();
         if (newPC < 0x9000)
         {
            char s[100];
            sprintf(s, "Bad address %X jumped to from %X", newPC, pc);
            throw TempestException(s);
         }
      }

      processorStatus = "Exited normally";
   }
   catch (CPU6502Exception &_x6502)
   {
      processorStatus = _x6502.what();
   }
   catch (TempestException &_xTempest)
   {
      // for now this goes as the processor status, too
      processorStatus = _xTempest.what();
   }
   catch (...)
   {
      processorStatus = "Tempest runner unknown exception";
   }

   state = Terminated;
}


/// <summary>
/// Synchronizes the CPU with realtime such that the CPU executes instructions
/// at a rate of 1.5MHz, the same as the actual Tempest 6502 CPU.  It does this
/// by doing a thread sleep whenever time measured by CPU cycles gets ahead of
/// system time.
/// </summary>
void CPU6502Runner::SynchronizeCPUWithRealTime(void)
{
   // we get called every millisecond according to the number of clock cycles
   // executed by the 6502... our job is to align that with real time by pausing
   // to let realtime catch up
   cpuTime += std::chrono::microseconds(1000);

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
   if (cpuAheadTime.count() > 0)
      std::this_thread::sleep_for(cpuAheadTime);
}


