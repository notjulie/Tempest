
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

   TDNWin32TempestVectorIO::TDNWin32TempestVectorIO(void)
   {
      tempestVectorIO = new Win32TempestVectorIO();
   }

   TDNWin32TempestVectorIO::~TDNWin32TempestVectorIO(void)
   {
      delete tempestVectorIO, tempestVectorIO = NULL;
   }

   VectorEnumerator ^TDNWin32TempestVectorIO::GetVectorEnumerator(void)
	{
		std::vector<SimpleVector> vectorList;
		tempestVectorIO->GetVectorList(vectorList);
		return gcnew VectorEnumerator(vectorList);
	}

};
