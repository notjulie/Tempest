
#include "stdafx.h"
#include "Win32TempestEnvironment.h"


#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

#ifdef _WIN32
   #pragma warning(pop)
#endif

Win32TempestEnvironment::Win32TempestEnvironment(void)
{
}

Win32TempestEnvironment::~Win32TempestEnvironment(void)
{
}

std::string Win32TempestEnvironment::GetDatabasePathName(void)
{
   // get this EXE's file name
   char exeFileName[1000] = "";
   GetModuleFileNameA(NULL, exeFileName, sizeof(exeFileName));

   // strip off the file name
   size_t len = strlen(exeFileName);
   while (len > 0 && exeFileName[len - 1] != '\\')
      exeFileName[--len] = 0;

   // return the result
   std::string result = exeFileName;
   result += "Tempest.DB";
   return result;
}
