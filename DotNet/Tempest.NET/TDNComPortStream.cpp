
#include "Stdafx.h"

#include "TempestCPU/TempestException.h"
#include "Win32/TempestWin32/Win32ComPortStream.h"

#include "TDNComPortStream.h"

namespace TempestDotNET {

   TDNComPortStream::TDNComPortStream(String ^_name)
   {
      try
      {
         std::string name;
         for (int i = 0; i < _name->Length; ++i)
            name += (char)_name[i];
         stream = new Win32ComPortStream(name.c_str());
      }
      catch (TempestException &x)
      {
         throw gcnew Exception(gcnew String(x.what()));
      }
   }

   TDNComPortStream::~TDNComPortStream(void)
   {
      delete stream, stream = NULL;
   }

};
