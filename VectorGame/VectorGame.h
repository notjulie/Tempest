// ====================================================================
// Vector game emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is an abstraction of a vector game, such as Asteroids or Tempest
// ====================================================================


#ifndef VECTORGAME_H
#define VECTORGAME_H

#include <vector>
#include "CPU6502Runner.h"
#include "SimpleVector.h"

class AbstractArcadeGameControlPanelReader;
class AbstractBus;
class AbstractTempestSoundOutput;

class VectorGame
{
public:
   VectorGame(void);
   virtual ~VectorGame(void);

   AbstractBus *GetBus(void) { return cpuRunner.GetBus(); }
   CPU6502Runner *GetCPURunner(void) { return &cpuRunner; }

   virtual void GetAllVectors(std::vector<SimpleVector> &vectors) = 0;
   virtual void SetControlPanel(AbstractArcadeGameControlPanelReader *controlPanel) = 0;
   virtual void SetSoundOutput(AbstractTempestSoundOutput *soundOutput) = 0;
   virtual void SingleStep(void) { cpuRunner.SingleStep(); }
   virtual void Start(void) {}

protected:
   void SetBus(AbstractBus *_bus) { cpuRunner.SetBus(_bus); }

private:
   CPU6502Runner cpuRunner;
};

#endif
