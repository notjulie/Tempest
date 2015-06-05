
#ifndef ITDNSTREAMPROVIDER_H
#define ITDNSTREAMPROVIDER_H

using namespace System;
using namespace System::Threading;

class AbstractTempestStream;

namespace TempestDotNET {

   public interface class ITDNStreamProvider {
   public:
      AbstractTempestStream *GetStream(void);
   };


   public ref class TDNStreamPointer : public ITDNStreamProvider {
   public:
      TDNStreamPointer(Object ^owner, AbstractTempestStream *_stream) {
         // save a reference to the owner so that the stream doesn't get collected
         // and of course save the stream pointer
         streamOwner = owner;
         stream = _stream;
      }

      virtual AbstractTempestStream *GetStream(void) { return stream; }

   private:
      Object ^streamOwner;
      AbstractTempestStream *stream;
   };
};



#endif
