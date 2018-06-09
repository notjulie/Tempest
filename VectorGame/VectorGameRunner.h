// ====================================================================
// Vector game emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is a thread class that drives a VectorGame.
// ====================================================================


#ifndef VECTORGAMERUNNER_H
#define VECTORGAMERUNNER_H

#include <stdint.h>
#include <string>
#include "CPU6502Runner.h"

class VectorGame;

class VectorGameRunner final
{
public:
   VectorGameRunner(VectorGame *game);
   ~VectorGameRunner(void);

   void Start(void);

   // simple dispatches to the runner
   uint8_t  GetAccumulator(void) { return game->GetCPURunner()->Get6502()->GetA(); }
   int      GetProgramCounter(void) { return game->GetCPURunner()->Get6502()->GetPC(); }
   uint8_t  GetXRegister(void) { return game->GetCPURunner()->Get6502()->GetX(); }
   uint8_t  GetYRegister(void) { return game->GetCPURunner()->Get6502()->GetY(); }
   void     Step(void) { game->GetCPURunner()->Step(); }
   void     Resume(void) { game->GetCPURunner()->Resume(); }
   bool     IsStopped(void) const { return game->GetCPURunner()->IsStopped(); }
   void     SetBreakpoint(uint16_t address, bool set) { game->GetCPURunner()->SetBreakpoint(address, set); }
   bool     IsTerminated(void) const { return isTerminated; }
   std::string GetProcessorStatus() const { return game->GetCPURunner()->GetProcessorStatus(); }

private:
   void	RunnerThread(void);

private:
   VectorGame * game;
   bool     terminateRequested = false;
   bool     isTerminated = false;

   /// <summary>
   /// this is actually a pointer to a std::thread, but the .NET CLR doesn't allow including
   /// <thread> in any file it compiles, so we leave the detail private to the CPP file
   /// </summary>
   void *theThread = nullptr;
};


#endif
