
#ifndef TDNTEMPESTIOSTREAMLISTENER_H
#define TDNTEMPESTIOSTREAMLISTENER_H

using namespace System;
using namespace System::Threading;

#include "TDNMemoryStream.h"
#include "TDNWin32TempestIO.h"

class TempestIOStreamListener;

namespace TempestDotNET {

   public ref class TDNTempestIOStreamListener : public IDisposable {
	public:
      TDNTempestIOStreamListener(ITDNStreamProvider ^stream, TDNWin32TempestSoundIO ^tempestIO);
		~TDNTempestIOStreamListener(void);

   private:
      void ThreadEntry();

	private:
      ITDNStreamProvider ^stream;
		TDNWin32TempestSoundIO ^tempestIO;

		TempestIOStreamListener *listener;

      bool terminated;
      Thread ^thread;
	};
};


#endif

