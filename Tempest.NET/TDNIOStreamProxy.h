
#ifndef TDNIOSTREAMPROXY_H
#define TDNIOSTREAMPROXY_H

using namespace System;
using namespace System::Threading;

#include "TDNMemoryStream.h"

namespace TempestDotNET {

	public ref class TDNIOStreamProxy {
	public:
		TDNIOStreamProxy(TDNMemoryStream ^stream);
	};
};

#endif
