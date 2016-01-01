
#include "stdafx.h"
#include <stdio.h>
#include <pthread.h>

#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestRunner.h"
#include "TempestIO/TempestIOStreamProxy.h"

#include "PiSerialStream.h"
#include "TempestPiEnvironment.h"
#include "TempestPiIO.h"


static void Run(void);

int main()
{
   try
   {
      Run();
   }
   catch (TempestException &te)
   {
      printf("%s\n", te.what());
      getchar();
   }
   catch (const char *s)
   {
      printf("%s\n", s);
      getchar();
   }
}

static void Run(void)
{
    // create our peripherals
    TempestPiEnvironment environment;
    TempestPiIO vectorIO;
    PiSerialStream serialStream;
    TempestIOStreamProxy soundIO(&serialStream);

    // create the runner object that drives the fake 6502
    TempestRunner tempestRunner(&environment);
    tempestRunner.SetTempestIO(&soundIO, &vectorIO);

    // go
    tempestRunner.Start();

   // push to the screen
   timespec startTime;
   clock_gettime(CLOCK_REALTIME, &startTime);
   int framesPerSecond = 0;
   for (;;)
   {
      vectorIO.PushFrameToScreen();
      ++framesPerSecond;

      timespec now;
      clock_gettime(CLOCK_REALTIME, &now);
      if (now.tv_sec != startTime.tv_sec)
      {
         printf("%d\n", framesPerSecond);
         framesPerSecond = 0;
         startTime = now;
      }
   }

    sleep(10);
    uint64_t clockCycles = tempestRunner.GetTotalClockCycles();
    printf("Total clock cycles: %d\n", (int)clockCycles);
}
