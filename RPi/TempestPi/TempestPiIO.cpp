
#include "stdafx.h"

#include "TempestPiIO.h"


TempestPiIO::TempestPiIO(void)
{
   logFrameRate = false;
   terminated = false;
   vectorGoCount = 0;
   vectorResetCount = 0;
   lastVectorRAMWrite = 0;

   vectorLists.resize(20);
   mostRecentVectors = 0;
}


void TempestPiIO::Run(void)
{
   // push to the screen
   timespec startTime;
   clock_gettime(CLOCK_REALTIME, &startTime);
   int framesPerSecond = 0;
   uint64_t lastVectorGoCount = vectorGoCount;
   uint64_t lastVectorResetCount = vectorResetCount;

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
                   "%d,%d,%d\n",
                   framesPerSecond,
                   (int)(vectorGoCount - lastVectorGoCount),
                   (int)(vectorResetCount - lastVectorResetCount)
                   );
         lastVectorGoCount = vectorGoCount;
         lastVectorResetCount = vectorResetCount;
         framesPerSecond = 0;
         startTime = now;
      }
   }
}

void TempestPiIO::PushFrameToScreen(void)
{
   // dump them to the screen
   screen.DisplayVectors(vectorLists[mostRecentVectors]);
}


void TempestPiIO::WriteVectorRAM(uint16_t address, uint8_t value, uint64_t cpuTime)
{
   // do the write
   vectorInterpreter.WriteVectorRAM(address, value);

   // if it's been a while since the last write we assume that the vector
   // data is stable and now is a good time to process it
   if (cpuTime - lastVectorRAMWrite > 800)
      ProcessVectorData();
   lastVectorRAMWrite = cpuTime;
}

void TempestPiIO::ProcessVectorData(void)
{
   unsigned nextIndex = mostRecentVectors + 1;
   if (nextIndex >= vectorLists.size())
      nextIndex = 0;
   vectorInterpreter.ClearVectors();
   vectorInterpreter.Reset();
   vectorInterpreter.Go();
   vectorInterpreter.Interpret();
   vectorInterpreter.GetAllVectors(vectorLists[nextIndex]);

   // set this vector list as the new official list to display
   mostRecentVectors = nextIndex;
}

void TempestPiIO::VectorGo(void)
{
   // make a note of it for diagnostic purposes
   ++vectorGoCount;
}

void TempestPiIO::VectorReset(void)
{
   ++vectorResetCount;
}

bool TempestPiIO::IsVectorHalt(void)
{
   return true;
}

