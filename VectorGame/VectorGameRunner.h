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

class CPU6502Runner;
class VectorGame;

class VectorGameRunner final
{
public:
   VectorGameRunner(VectorGame *game);
   ~VectorGameRunner(void);

   void Start(void);

   // simple dispatches to the runner
   uint8_t  GetAccumulator(void) { return cpuRunner->Get6502()->GetA(); }
   int      GetProgramCounter(void) { return cpuRunner->Get6502()->GetPC(); }
   uint8_t  GetXRegister(void) { return cpuRunner->Get6502()->GetX(); }
   uint8_t  GetYRegister(void) { return cpuRunner->Get6502()->GetY(); }
   void     Step(void) { cpuRunner->Step(); }
   void     Resume(void) { cpuRunner->Resume(); }
   bool     IsStopped(void) const { return cpuRunner->IsStopped(); }
   void     SetBreakpoint(uint16_t address, bool set) { cpuRunner->SetBreakpoint(address, set); }
   bool     IsTerminated(void) const { return cpuRunner->IsTerminated(); }
   std::string GetProcessorStatus() const { return cpuRunner->GetProcessorStatus(); }

private:
   VectorGame * game;
   CPU6502Runner *cpuRunner = nullptr;
};


#endif
