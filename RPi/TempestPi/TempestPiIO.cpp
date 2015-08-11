
#include "stdafx.h"

#include "TempestPiIO.h"


void TempestPiIO::PushFrameToScreen(void)
{
   // get the vectors
   std::vector<PiVector> vectors;
   vectorInterpreter.ClearVectors();
   vectorInterpreter.Interpret();
   vectorInterpreter.GetAllVectors(vectors);

   // dump them to the screen
   screen.DisplayVectors(vectors);
}
