
#ifndef TDNMIRRORINGSTREAM_H
#define TDNMIRRORINGSTREAM_H

using namespace System;
using namespace System::Threading;

#include "TempestIO/MirroringStream.h"
#include "ITDNStreamProvider.h"

class Win32ComPortStream;

namespace TempestDotNET {

   public ref class TDNMirroringStream : public ITDNStreamProvider {
   public:
      TDNMirroringStream(ITDNStreamProvider ^target, ITDNStreamProvider ^mirror);
      ~TDNMirroringStream(void);

      virtual AbstractTempestStream *GetStream(void) { return stream; }

   private:
      ITDNStreamProvider ^target;
      ITDNStreamProvider ^mirror;
      MirroringStream *stream;
   };
};

#endif
