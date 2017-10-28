
#include "stdafx.h"

#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestException.h"

#include "AsteroidsRunner.h"

AsteroidsRunner::AsteroidsRunner(AbstractTempestEnvironment *_environment)
	:
		asteroidsBus(_environment)
{
   try
   {
      // save parameters
      environment = _environment;

      // register commands
      environment->RegisterCommand(
         "?pc",
         [this](const CommandLine &) {
         std::ostringstream s;
         s << std::setfill('0') << std::hex << std::uppercase;
         s << "PC: " << std::setw(4) << cpuRunner->Get6502()->GetPC();
         return s.str();
      }
      );

      // create the CPU runner
      cpuRunner = CPU6502Runner::Create(&asteroidsBus);
   }
   catch (...)
   {
      delete cpuRunner;
      cpuRunner = nullptr;
      throw;
   }
}

AsteroidsRunner::~AsteroidsRunner(void)
{
   // stop the runner thread before anything else
   delete cpuRunner;
   cpuRunner = nullptr;
}


