
#include "stdafx.h"

#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestException.h"
#include "AsteroidsVectorInterpreter.h"

#include "AsteroidsGame.h"

AsteroidsGame::AsteroidsGame(AbstractTempestEnvironment *_environment)
	:
      asteroidsBus(_environment)
{
   // save parameters
   environment = _environment;

   // solder the CPU to the bus
   cpuRunner.SetBus(&asteroidsBus);
}

AsteroidsGame::~AsteroidsGame(void)
{
}

void AsteroidsGame::GetAllVectors(std::vector<SimpleVector> &vectors)
{
   if (asteroidsBus.IsVectorGo())
   {
      // get the latest vector data
      AsteroidsVectorInterpreter vectorInterpreter;
      asteroidsBus.GetVectorData(vectorInterpreter);

      // interpret it
      vectorInterpreter.Interpret();
      vectorInterpreter.GetAllVectors(vectors);
   }
}


