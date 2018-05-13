
#ifndef TDNTEMPESTIOSTREAMLISTENER_H
#define TDNTEMPESTIOSTREAMLISTENER_H

using namespace System;
using namespace System::Threading;

#include "TDNMemoryStream.h"
#include "Win32TempestIO.h"

class TempestIOStreamListener;

namespace TempestDotNET {

   public ref class TDNTempestIOStreamListener : public IDisposable {
	public:
      TDNTempestIOStreamListener(ITDNStreamProvider ^stream);
		~TDNTempestIOStreamListener(void);

   private:
      void ThreadEntry();

	private:
      ITDNStreamProvider ^stream;
      Win32TempestSoundIO *tempestSoundIO = nullptr;

		TempestIOStreamListener *listener;

      bool terminated;
      Thread ^thread;
	};
};


#endif

