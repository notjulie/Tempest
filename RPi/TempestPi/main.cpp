
#include "stdafx.h"
#include <stdio.h>
#include <string.h>

#include "TempestCPU/TempestException.h"

#include "TempestPi.h"


int main(int argc, char **argv)
{
   try
   {
      TempestPi tempest;
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

