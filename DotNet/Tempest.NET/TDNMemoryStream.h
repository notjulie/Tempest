
#ifndef TDNMEMORYSTREAM_H
#define TDNMEMORYSTREAM_H

using namespace System;
using namespace System::Threading;

class TempestMemoryStream;

namespace TempestDotNET {

	public ref class TDNMemoryStream {
	public:
		TDNMemoryStream(void);
		~TDNMemoryStream(void);

		TempestMemoryStream *GetStream(void) { return stream; }

	private:
		TempestMemoryStream *stream;
	};
};

#endif
