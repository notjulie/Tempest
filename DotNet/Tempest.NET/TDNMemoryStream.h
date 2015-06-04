
#ifndef TDNMEMORYSTREAM_H
#define TDNMEMORYSTREAM_H

using namespace System;
using namespace System::Threading;

#include "ITDNStreamProvider.h"

class TempestMemoryStream;

namespace TempestDotNET {

	public ref class TDNMemoryStream {
	public:
		TDNMemoryStream(void);
		~TDNMemoryStream(void);

      ITDNStreamProvider ^GetLeftSide();
      ITDNStreamProvider ^GetRightSide();

	private:
		TempestMemoryStream *stream;
	};
};

#endif
