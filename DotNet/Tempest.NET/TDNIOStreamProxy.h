
#ifndef TDNIOSTREAMPROXY_H
#define TDNIOSTREAMPROXY_H

using namespace System;
using namespace System::Threading;

#include "TempestIO/TempestIOStreamProxy.h"

#include "TDNMemoryStream.h"

class AbstractTempestIO;

namespace TempestDotNET {

	public ref class TDNIOStreamProxy {
	public:
      TDNIOStreamProxy(ITDNStreamProvider ^stream);
		~TDNIOStreamProxy(void);


		AbstractTempestIO *GetIOObject(void) { return tempestIO; }

	private:
      ITDNStreamProvider ^managedStream;
		TempestIOStreamProxy *tempestIO;
	};
};

#endif
