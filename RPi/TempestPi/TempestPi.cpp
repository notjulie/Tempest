
#include "stdafx.h"
#include <pthread.h>

#include "AsteroidsGame.h"
#include "TempestCPU/TempestException.h"
#include "TempestIO/TempestIOStreamProxy.h"
#include "TempestGame.h"
#include "VectorGameRunner.h"
#include "VectorMultiGame.h"

#include "PiSerialStream.h"

#include "TempestPi.h"


TempestPi::TempestPi(void)
{
   // clear
   currentCommand[0] = 0;

   // open the log
   log = fopen("TempestMonitor.log", "a");
   fprintf(log, "\n\n\n=====================================================\n");
   fprintf(log, "=====================================================\n");
   fprintf(log, "=====================================================\n");

   // register our commands
   environment.RegisterCommand(
      "exit",
      [this](const CommandLine &command) {
         vectorIO.Terminate();
         return "OK";
         }
      );

   // register the database for Tempest
   tempestDB.Open("./TempestDB");
   environment.RegisterResource(AbstractTempestDB::ResourceID(), &tempestDB);

   // create the game
   game = new VectorMultiGame(
      &environment,
      {
         [this]() { return new TempestGame(&environment); },
         [this]() { return new AsteroidsGame(&environment); }
      }
   );

   // create the runner; it doesn't get started until later
   gameRunner = new VectorGameRunner(game);

   // start our socket listener
   socketListener = new TempestSocketListener(&environment);
}

TempestPi::~TempestPi(void)
{
   // start by terminating our threads
   terminated = true;
   if (monitorThread != NULL)
   {
      monitorThread->join();
      delete monitorThread, monitorThread = NULL;
   }

   // the socket listener can make calls almost anywhere, so we need to
   // shut it down before we start tearing down the rest of the app
   if (socketListener != NULL)
      delete socketListener, socketListener = NULL;

   // the runner is also a thread and it needs to get shut down before
   // we delete objects it uses
   delete gameRunner, gameRunner = nullptr;
   delete game, game = nullptr;

   delete soundIO, soundIO = NULL;
   delete serialStream, serialStream = NULL;
}

void TempestPi::Run(void)
{
   try
   {
      // create our peripherals
      serialStream = new PiSerialStream();
      environment.RegisterCommand(
         "?serReadSts",
         [this](const CommandLine &command) {
            return std::string(serialStream->GetReadStatus()) + ": " + std::to_string(serialStream->GetReadCount());
            }
         );
      environment.RegisterCommand(
         "?serWriteSts",
         [this](const CommandLine &command) {
            return std::string(serialStream->GetWriteStatus()) + ": " + std::to_string(serialStream->GetWriteCount());
            }
         );

       soundIO = new TempestIOStreamProxy(serialStream);

       // hook the game to its ins and outs
       game->SetSoundOutput(soundIO);
       game->SetControlPanel(soundIO);

      // start the monitor thread if it's not running
      if (monitorThread == NULL)
         monitorThread = new std::thread(
               [this]() { MonitorThread(); }
            );

       // go
       gameRunner->Start();

      // the IO object (i.e. the screen) takes over the main thread
      // from here
      vectorIO.Run(game);
   }
   catch (TempestException &te)
   {
      printf("%s\n", te.what());
   }
   catch (const char *s)
   {
      printf("%s\n", s);
   }
   catch (...)
   {
      printf("Unknown exception caught in TempestPi::Run\n");
   }
}

void TempestPi::MonitorThread(void)
{
   char s[200];

   Log("Monitor running");

   while (!terminated)
   {
      // pause
      usleep(100000);

      // look for any issues
      if (gameRunner->IsTerminated())
      {
         Log("Tempest terminated");
         Log(gameRunner->GetProcessorStatus().c_str());

         sprintf(s, "Program terminated at address %X", gameRunner->GetProgramCounter());
         Log(s);
         sprintf(s, "A=%X, X=%X, Y=%X",
                 gameRunner->GetAccumulator(),
                 gameRunner->GetXRegister(),
                 gameRunner->GetYRegister()
                 );
         Log(s);

         return;
      }
   }
}

void TempestPi::Log(const char *s)
{
   printf("%s\n", s);
   fprintf(log, "%s\n", s);
   fflush(log);
}



