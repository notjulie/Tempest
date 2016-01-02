
#include "stdafx.h"

#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestRunner.h"
#include "TempestIO/TempestIOStreamProxy.h"

#include "PiSerialStream.h"
#include "TempestPiEnvironment.h"
#include "TempestPiIO.h"

#include "TempestPi.h"


TempestPi::TempestPi(void)
{
   demo = false;
}

void TempestPi::Run(void)
{
   try
   {
       // create our peripherals
       TempestPiEnvironment environment;
       TempestPiIO vectorIO;
       PiSerialStream serialStream;
       TempestIOStreamProxy soundIO(&serialStream);

       // create the runner object that drives the fake 6502
       TempestRunner tempestRunner(&environment);
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
