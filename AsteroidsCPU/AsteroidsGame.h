
#ifndef ASTEROIDSRUNNER_H
#define ASTEROIDSRUNNER_H

#include <chrono>
#include "AsteroidsBus.h"
#include "VectorGame.h"

class AbstractTempestEnvironment;
class AsteroidsVSM;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class AsteroidsGame : public VectorGame
{
public:
   AsteroidsGame(AbstractTempestEnvironment *environment);
   ~AsteroidsGame(void);
   AsteroidsGame(const AsteroidsGame &tr) = delete;
   AsteroidsGame &operator=(const AsteroidsGame &tr) = delete;

	// simple dispatches to the AsteroidsBus object
   void     SetControlPanel(AbstractArcadeGameControlPanelReader *controlPanel) { asteroidsBus.SetControlPanel(controlPanel); }
   void     SetSoundOutput(AbstractTempestSoundOutput *soundOutput) { asteroidsBus.SetSoundOutput(soundOutput); }

   // base class overrides
   virtual void GetAllVectors(std::vector<SimpleVector> &vectors);
   virtual CPU6502Runner *GetCPURunner(void) { return &cpuRunner; }
   virtual void SingleStep(void) { cpuRunner.SingleStep(); }

private:
	AbstractTempestEnvironment *environment;
	AsteroidsBus	asteroidsBus;
   CPU6502Runner cpuRunner;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
