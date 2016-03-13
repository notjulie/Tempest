
#include "stdafx.h"
#include <pthread.h>

#include "TempestCPU/TempestException.h"
#include "TempestIO/TempestIOStreamProxy.h"

#include "PiSerialStream.h"

#include "TempestPi.h"


TempestPi::TempestPi(void)
   :
      tempestRunner(&environment),
      socketListener(&environment)
{
   // clear
   demo = false;
   terminated = false;
   monitorThread = 0;
   keyboardThread = 0;
   log = NULL;
   currentCommand[0] = 0;

   // open the log
   log = fopen("TempestMonitor.log", "a");
   fprintf(log, "\n\n\n=====================================================\n");
   fprintf(log, "=====================================================\n");
   fprintf(log, "=====================================================\n");
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
       // create our peripherals
       PiSerialStream serialStream;
       TempestIOStreamProxy soundIO(&serialStream);

       // create the runner object that drives the fake 6502
       tempestRunner.SetTempestIO(&soundIO, &vectorIO);
       if (demo)
         tempestRunner.SetDemoMode();

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

      // start the keyboard thread if it's not running
      if (keyboardThread == 0)
      {
         int result = pthread_create(
            &keyboardThread,
            NULL,
            &KeyboardThreadEntry,
            this
            );
         if (result != 0)
            throw TempestException("Error creating keyboard thread");
         pthread_setname_np(keyboardThread, "Keyboard");
      }

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
   char s[200];

   Log("Monitor running");

   while (!terminated)
   {
      // pause
      usleep(100000);

      // look for any issues
      if (tempestRunner.IsTerminated())
      {
         Log("Tempest terminated");
         Log(tempestRunner.GetProcessorStatus().c_str());

         sprintf(s, "Program terminated at address %X", tempestRunner.GetProgramCounter());
         Log(s);
         sprintf(s, "A=%X, X=%X, Y=%X, S=%X",
                 tempestRunner.GetAccumulator(),
                 tempestRunner.GetXRegister(),
                 tempestRunner.GetYRegister(),
                 tempestRunner.GetStackPointer()
                 );
         Log(s);

         return;
      }
   }
}

void TempestPi::KeyboardThread(void)
{
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
         continue;
      }

      // add it to our buffer
      int len = strlen(currentCommand);
      currentCommand[len++] = c;
      currentCommand[len] = 0;
      if (len >= (int)sizeof(currentCommand) - 3)
      {
         printf("Input overflow, ignored\n");
         currentCommand[0] = 0;
      }
   }
}

void TempestPi::ProcessCommand(const char *command)
{
   if (strcasecmp(command, "demo") == 0)
   {
      tempestRunner.SetDemoMode();
      return;
   }

   if (strcasecmp(command, "logframerate") == 0)
   {
      vectorIO.LogFrameRate();
      return;
   }
}

void TempestPi::Log(const char *s)
{
   printf("%s\n", s);
   fprintf(log, "%s\n", s);
   fflush(log);
}


void *TempestPi::KeyboardThreadEntry(void *pThis)
{
   TempestPi *tempest = (TempestPi*)pThis;

   try
   {
      tempest->KeyboardThread();
      tempest->Log("Keyboard thread exit");
   }
   catch (...)
   {
      tempest->Log("Keyboard thread unhandled exception");
   }

   return NULL;
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

