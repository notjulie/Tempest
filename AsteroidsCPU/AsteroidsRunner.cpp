
#include "stdafx.h"

#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestException.h"

#include "AsteroidsRunner.h"

AsteroidsRunner::AsteroidsRunner(AbstractTempestEnvironment *_environment)
	:
		asteroidsBus(_environment)
{
	// save parameters
	environment = _environment;

   // initialize the child class
   SetBus(&asteroidsBus);

   // register commands
   environment->RegisterCommand(
      "?pc",
      [this](const CommandLine &) {
            std::ostringstream s;
            s << std::setfill('0') << std::hex << std::uppercase;
            s << "PC: " << std::setw(4) << Get6502()->GetPC();
            return s.str();
         }
      );
}

AsteroidsRunner::~AsteroidsRunner(void)
{
}


