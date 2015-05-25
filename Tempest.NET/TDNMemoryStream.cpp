
#include "Stdafx.h"

#include "../TempestIO/TempestMemoryStream.h"

#include "TDNMemoryStream.h"

namespace TempestDotNET {

	TDNMemoryStream::TDNMemoryStream(void)
	{
		stream = new TempestMemoryStream();
	}

	TDNMemoryStream::~TDNMemoryStream(void)
	{
		delete stream, stream = NULL;
	}
};
