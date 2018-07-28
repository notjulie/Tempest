
#include "Tempest.NET.h"

#include "AsteroidsCPU/AsteroidsGame.h"
#include "TempestCPU/TempestGame.h"
#include "VectorMultiGame.h"

#include "NativeVectorGameManager.h"


VectorGame *CreateOurVectorGame(AbstractGameEnvironment *environment)
{
   // we have to do this outside of the class because of a rule about C++ lambdas
   // in managed code
   return new VectorMultiGame(
      environment,
      {
         [=]() { return new TempestGame(environment); },
         [=]() { return new AsteroidsGame(environment); }
      }
   );
}

