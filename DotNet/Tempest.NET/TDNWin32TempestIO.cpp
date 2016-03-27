
#include "stdafx.h"

#include "VectorEnumerator.h"
#include "Win32TempestIO.h"

#include "TDNWin32TempestIO.h"


using namespace System;

namespace TempestDotNET {
   TDNWin32TempestSoundIO::TDNWin32TempestSoundIO(void)
   {
      tempestSoundIO = new Win32TempestSoundIO();
   }

   TDNWin32TempestSoundIO::~TDNWin32TempestSoundIO(void)
   {
      delete tempestSoundIO, tempestSoundIO = NULL;
   }
};
