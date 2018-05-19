// ====================================================================
// Vector game emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is a thread class that drives a VectorGame.
// ====================================================================


#include "CPU6502Runner.h"
#include "VectorGame.h"
#include "VectorGameRunner.h"

VectorGameRunner::VectorGameRunner(VectorGame *_game)
{
   // copy parameters
   game = _game;

   // create objects
   cpuRunner = CPU6502Runner::Create(game->GetBus());
}

VectorGameRunner::~VectorGameRunner(void)
{
   // stop the runner thread
   delete cpuRunner;
   cpuRunner = nullptr;
}


void VectorGameRunner::Start(void)
{
   // tell the game to start
   game->Start();

   // tell it to register its hooks
   game->Register6502Hooks(cpuRunner);

   // start running the CPU
   cpuRunner->Start();
}


