
#include "Stdafx.h"

#include "TempestIO/TempestIOStreamProxy.h"

#include "TDNIOStreamProxy.h"

namespace TempestDotNET {

	TDNIOStreamProxy::TDNIOStreamProxy(ITDNStreamProvider ^stream)
	{
		// keep a managed reference to the stream so that it doesn't get
		// garbage collected while we're using it
		managedStream = stream;

		// create our native proxy object
      tempestIO = new TempestIOStreamProxy(managedStream->GetStream());
	}


	TDNIOStreamProxy::~TDNIOStreamProxy(void)
	{
		delete tempestIO, tempestIO = NULL;
	}
};
