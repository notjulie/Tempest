
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestRunner.h"
#include "TempestIO/TempestIOStreamProxy.h"

#include "PiSerialStream.h"
#include "TempestPiEnvironment.h"
#include "TempestPiIO.h"


static void Run(bool demo);


int main(int argc, char **argv)
{
   bool isDemo = false;

   for (int i=0; i<argc; ++i)
      if (strcmp(argv[i], "demo") == 0)
         isDemo = true;

   try
   {
      Run(isDemo);
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

static void Run(bool demo)
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
