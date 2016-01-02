
#include "stdafx.h"
#include <stdio.h>
#include <string.h>

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

   TempestPi tempest;
   tempest.SetDemoMode(isDemo);
   tempest.Run();
}

