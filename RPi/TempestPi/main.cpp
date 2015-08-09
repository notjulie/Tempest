
#include "stdafx.h"
#include <stdio.h>
#include <pthread.h>

#include "TempestCPU/TempestRunner.h"

#include "TempestPiEnvironment.h"
#include "TempestPiIO.h"



int main()
{
    // create our peripherals
    TempestPiEnvironment environment;
    TempestPiIO io;

    // create the runner object that drives the fake 6502
    TempestRunner tempestRunner(&environment);
    tempestRunner.SetTempestIO(&io);

    // go
    tempestRunner.Start();

   // push to the screen
   timespec startTime;
   clock_gettime(CLOCK_REALTIME, &startTime);
   int framesPerSecond = 0;
   for (;;)
   {
      io.PushFrameToScreen();
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
