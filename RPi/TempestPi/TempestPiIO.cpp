
#include "stdafx.h"

#include "../../TempestIO/Vector/SimpleVectorDataInterpreter.h"
#include "../../TempestIO/Vector/VectorData.h"
#include "VectorGame.h"

#include "TempestPiIO.h"


TempestPiIO::TempestPiIO(void)
{
}


void TempestPiIO::Run(VectorGame *game)
{
   this->game = game;

   // push to the screen
   timespec startTime;
   clock_gettime(CLOCK_REALTIME, &startTime);
   int framesPerSecond = 0;

   while (!terminated)
   {
      PushFrameToScreen();
      ++framesPerSecond;

      timespec now;
      clock_gettime(CLOCK_REALTIME, &now);
      if (now.tv_sec != startTime.tv_sec)
      {
         if (logFrameRate)
            printf(
                   "%d\n",
                   framesPerSecond
                   );
         framesPerSecond = 0;
         startTime = now;
      }
   }
}

void TempestPiIO::PushFrameToScreen(void)
{
   // get the vectors
   std::vector<DisplayVector> vectors;
   game->GetAllVectors(vectors);

   // dump them to the screen
   screen.DisplayVectors(vectors);
}


