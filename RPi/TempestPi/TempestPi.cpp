
#include "stdafx.h"
#include <pthread.h>

#include "TempestCPU/TempestException.h"
#include "TempestIO/TempestIOStreamProxy.h"

#include "PiSerialStream.h"

#include "TempestPi.h"


TempestPi::TempestPi(void)
{
   // clear
   demo = false;
   terminated = false;
   monitorThread = 0;
   log = NULL;
   currentCommand[0] = 0;
   tempestRunner = NULL;

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

   // create the runner; it doesn't get started until later
   tempestRunner = new TempestRunner(&environment);

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
   if (tempestRunner != NULL)
      delete tempestRunner, tempestRunner = NULL;

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

       // create the runner object that drives the fake 6502
       tempestRunner->SetTempestIO(soundIO);
       if (demo)
         tempestRunner->SetDemoMode();

      // start the monitor thread if it's not running
      if (monitorThread == NULL)
         monitorThread = new std::thread(
               [this]() { MonitorThread(); }
            );

       // go
       tempestRunner->Start();

      // the IO object (i.e. the screen) takes over the main thread
      // from here
      vectorIO.Run(tempestRunner);
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
      if (tempestRunner->IsTerminated())
      {
         Log("Tempest terminated");
         Log(tempestRunner->GetProcessorStatus().c_str());

         sprintf(s, "Program terminated at address %X", tempestRunner->GetProgramCounter());
         Log(s);
         sprintf(s, "A=%X, X=%X, Y=%X",
                 tempestRunner->GetAccumulator(),
                 tempestRunner->GetXRegister(),
                 tempestRunner->GetYRegister()
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



