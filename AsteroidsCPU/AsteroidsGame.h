
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
   bool     HaveVectorData(void) { return asteroidsBus.IsVectorGo(); }
   uint64_t GetTotalClockCycles(void) { return asteroidsBus.GetTotalClockCycles(); }
   void     GetVectorData(AsteroidsVectorInterpreter &vectorData) { asteroidsBus.GetVectorData(vectorData); }
   void     SetControlPanel(AbstractArcadeGameControlPanelReader *controlPanel) { asteroidsBus.SetControlPanel(controlPanel); }
   void     SetSoundOutput(AbstractTempestSoundOutput *soundOutput) { asteroidsBus.SetSoundOutput(soundOutput); }

   // base class overrides
   virtual AbstractBus *GetBus(void) { return &asteroidsBus; }

private:
	AbstractTempestEnvironment *environment;
	AsteroidsBus	asteroidsBus;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
