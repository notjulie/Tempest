
#include "stdafx.h"
#include <pthread.h>

#include "TempestCPU/TempestException.h"
#include "TempestIO/TempestIOStreamProxy.h"

#include "PiSerialStream.h"
#include "TempestPiIO.h"

#include "TempestPi.h"


TempestPi::TempestPi(void)
   :
      tempestRunner(&environment)
{
   // clear
   demo = false;
   terminated = false;
   monitorThread = 0;
}

TempestPi::~TempestPi(void)
{
   terminated = true;
   if (monitorThread != 0)
      pthread_join(monitorThread, NULL);
}

void TempestPi::Run(void)
{
   try
   {
      // start the monitor thread if it's not running
      if (monitorThread == 0)
      {
         int result = pthread_create(
            &monitorThread,
            NULL,
            &MonitorThreadEntry,
            this
            );
         if (result != 0)
            throw TempestException("Error creating Monitor thread");
         pthread_setname_np(monitorThread, "Monitor");
      }

       // create our peripherals
       TempestPiIO vectorIO;
       PiSerialStream serialStream;
       TempestIOStreamProxy soundIO(&serialStream);

       // create the runner object that drives the fake 6502
       tempestRunner.SetTempestIO(&soundIO, &vectorIO);
       if (demo)
         tempestRunner.SetDemoMode();

       // go
       tempestRunner.Start();

      // the IO object (i.e. the screen) takes over the main thread
      // from here
      vectorIO.Run();
   }
   catch (TempestException &te)
   {
      printf("%s\n", te.what());
   }
   catch (const char *s)
   {
      printf("%s\n", s);
   }
}

void TempestPi::MonitorThread(void)
{
   Log("Monitor running");

   char currentCommand [100] = "";

   while (!terminated)
   {
      // pause
      usleep(100000);

      // look for any issues
      if (tempestRunner.IsTerminated())
      {
         Log("Tempest terminated");
         Log(tempestRunner.GetProcessorStatus().c_str());
         return;
      }

      // check for incoming keyboard data
      for (;;)
      {
         // get a character
         int c = getc(stdin);
         if (c < 0)
            break;

         // if this is an enter process the command
         if (c=='\r' || c=='\n')
         {
            ProcessCommand(currentCommand);
            currentCommand[0] = 0;
         }

         // add it to our buffer
         int len = strlen(currentCommand);
         currentCommand[len++] = c;
         currentCommand[len] = 0;
         if (len >= sizeof(currentCommand) - 3)
         {
            Log("Input overflow, ignored");
            currentCommand[0] = 0;
         }
      }
   }
}

void TempestPi::ProcessCommand(const char *command)
{
   if (strcmp(command, "demo") == 0)
   {
      tempestRunner.SetDemoMode();
      return;
   }
}

void TempestPi::Log(const char *s)
{
   printf("%s\n", s);
}


void *TempestPi::MonitorThreadEntry(void *pThis)
{
   TempestPi *tempest = (TempestPi*)pThis;

   try
   {
      tempest->MonitorThread();
      tempest->Log("Monitor thread exit");
   }
   catch (...)
   {
      tempest->Log("Monitor thread unhandled exception");
   }

   return NULL;
}

