
#include "stdafx.h"
#include <stdio.h>
#include <string.h>

#include "TempestCPU/TempestException.h"

#include "TempestPi.h"


int main(int argc, char **argv)
{
   bool isDemo = false;

   for (int i=0; i<argc; ++i)
      if (strcmp(argv[i], "demo") == 0)
         isDemo = true;
#ifdef DEBUG
   isDemo = true;
#endif

   try
   {
      TempestPi tempest;
      tempest.SetDemoMode(isDemo);
      tempest.Run();
   }
   catch (TempestException &x)
   {
      printf("%s\n", x.what());
   }
   catch (const char *s)
   {
      printf("%s\n", s);
   }
   catch (...)
   {
      printf("Unknown exception\n");
   }
}

