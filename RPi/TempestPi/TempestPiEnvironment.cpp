
#include "stdafx.h"

#include "TempestCPU/TempestException.h"

#include "TempestPiEnvironment.h"


TempestPiEnvironment::TempestPiEnvironment(void)
{
}


std::string TempestPiEnvironment::GetDatabasePathName(void)
{
   return "./TempestDB";
}

