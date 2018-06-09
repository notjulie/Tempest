// ====================================================================
// Vector game emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is a thread class that drives a VectorGame.
// ====================================================================

#include <thread>
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
   if (theThread != nullptr)
   {
      terminateRequested = true;
      ((std::thread *)theThread)->join();
      delete (std::thread *)theThread;
      theThread = nullptr;
   }

   // delete our children
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

   // create the thread
   theThread = new std::thread(
      [this]() { RunnerThread(); }
   );
}


void VectorGameRunner::RunnerThread(void)
{
   while (!terminateRequested)
      cpuRunner->SingleStep();
   isTerminated = true;
}
