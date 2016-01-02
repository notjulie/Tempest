
#include "stdafx.h"

#include "TempestPiIO.h"


void TempestPiIO::Run(void)
{
   // push to the screen
   timespec startTime;
   clock_gettime(CLOCK_REALTIME, &startTime);
   int framesPerSecond = 0;
   for (;;)
   {
      PushFrameToScreen();
      ++framesPerSecond;

      timespec now;
      clock_gettime(CLOCK_REALTIME, &now);
      if (now.tv_sec != startTime.tv_sec)
      {
         //printf("%d\n", framesPerSecond);
         framesPerSecond = 0;
         startTime = now;
      }
   }
}

void TempestPiIO::PushFrameToScreen(void)
{
   // get the vectors
   std::vector<SimpleVector> vectors;
   vectorInterpreter.ClearVectors();
   vectorInterpreter.Interpret();
   vectorInterpreter.GetAllVectors(vectors);

   // dump them to the screen
   screen.DisplayVectors(vectors);
}
