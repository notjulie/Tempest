

#include "Stdafx.h"

#include "../TempestIO/TempestIOStreamListener.h"

#include "TDNTempestIOStreamListener.h"

namespace TempestDotNET {

	TDNTempestIOStreamListener::TDNTempestIOStreamListener(TDNMemoryStream ^stream, TDNWin32TempestIO ^tempestIO)
	{
		// keep references to the managed objects so they don't get garbage collected
		this->stream = stream;
		this->tempestIO = tempestIO;

		// create our listener
		listener = new TempestIOStreamListener();
	}


	TDNTempestIOStreamListener::~TDNTempestIOStreamListener(void)
	{
		delete listener, listener = NULL;
	}
};
