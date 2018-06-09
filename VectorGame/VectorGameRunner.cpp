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

#include "CPU6502Exception.h"
#include "CPU6502Runner.h"
#include "VectorGameException.h"
#include "VectorGame.h"

#include "VectorGameRunner.h"

VectorGameRunner::VectorGameRunner(VectorGame *_game)
{
   // copy parameters
   game = _game;
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
}


void VectorGameRunner::Start(void)
{
   // tell the game to start
   game->Start();

   // create the thread
   theThread = new std::thread(
      [this]() { RunnerThread(); }
   );
}


void VectorGameRunner::RunnerThread(void)
{
   try
   {
      while (!terminateRequested)
      {
         game->SingleStep();
      }

      processorStatus = "Exited normally";
   }
   catch (CPU6502Exception &_x6502)
   {
      processorStatus = _x6502.what();
   }
   catch (VectorGameException &_xTempest)
   {
      // for now this goes as the processor status, too
      processorStatus = _xTempest.what();
   }
   catch (...)
   {
      processorStatus = "Tempest runner unknown exception";
   }

   isTerminated = true;
}
