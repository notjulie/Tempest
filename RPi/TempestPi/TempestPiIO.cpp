
#include "stdafx.h"

#include "../../TempestCPU/TempestRunner.h"
#include "../../TempestIO/Vector/SimpleVectorDataInterpreter.h"
#include "../../TempestIO/Vector/VectorData.h"

#include "TempestPiIO.h"


TempestPiIO::TempestPiIO(void)
{
   logFrameRate = false;
   terminated = false;
   this->tempestRunner = NULL;
}


void TempestPiIO::Run(TempestRunner *tempestRunner)
{
   this->tempestRunner = tempestRunner;

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
   // get the vector data
   VectorData vectorData;
   tempestRunner->GetVectorData(vectorData);

   SimpleVectorDataInterpreter vectorInterpretor;
   vectorInterpretor.SetVectorData(vectorData);
   vectorInterpretor.Interpret();

   // dump them to the screen
   std::vector<SimpleVector> vectors;
   vectorInterpretor.GetAllVectors(vectors);
   screen.DisplayVectors(vectors);
}


