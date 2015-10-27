
#ifndef TEMPESTIODOTNET_H
#define TEMPESTIODOTNET_H

using namespace System;
using namespace System::Threading;

#include "VectorEnumerator.h"
#include "Win32TempestIO.h"

class Win32TempestIO;

namespace TempestDotNET {

	public ref class TDNWin32TempestIO {
	public:
		TDNWin32TempestIO(void);
		~TDNWin32TempestIO(void);

		VectorEnumerator ^GetVectorEnumerator(void);

      AbstractTempestVectorIO *GetVectorIOObject(void) { return tempestIO; }
      AbstractTempestSoundIO *GetSoundIOObject(void) { return tempestIO; }

	private:
		Win32TempestIO *tempestIO;
	};

};

#endif
