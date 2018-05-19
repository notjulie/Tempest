
#include "stdafx.h"

#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestException.h"

#include "AsteroidsGame.h"

AsteroidsGame::AsteroidsGame(AbstractTempestEnvironment *_environment)
	:
		asteroidsBus(_environment)
{
   // save parameters
   environment = _environment;
}

AsteroidsGame::~AsteroidsGame(void)
{
}


