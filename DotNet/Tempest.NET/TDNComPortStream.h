
#ifndef TDNCOMPORTSTREAM_H
#define TDNCOMPORTSTREAM_H

using namespace System;
using namespace System::Threading;

#include "Win32/TempestWin32/Win32ComPortStream.h"
#include "ITDNStreamProvider.h"

class Win32ComPortStream;

namespace TempestDotNET {

   public ref class TDNComPortStream : public ITDNStreamProvider {
   public:
      TDNComPortStream(String ^name);
      ~TDNComPortStream(void);

      virtual AbstractTempestStream *GetStream(void) { return stream; }

   private:
      Win32ComPortStream *stream;
   };
};

#endif
