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
class AbstractSoundGenerator;

class VectorGame
{
public:
   VectorGame(void);
   virtual ~VectorGame(void);

   virtual CPU6502Runner *GetCPURunner(void) = 0;

   virtual void GetAllVectors(std::vector<SimpleVector> &vectors) = 0;
   virtual void SetControlPanel(AbstractArcadeGameControlPanelReader *controlPanel) = 0;
   virtual void SetSoundOutput(AbstractSoundGenerator *soundOutput) = 0;
   virtual void SingleStep(void) = 0;
   virtual void Start(void) {}
};

#endif
