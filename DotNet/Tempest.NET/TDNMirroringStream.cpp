
#include "Stdafx.h"

#include "TDNMirroringStream.h"

namespace TempestDotNET {

   TDNMirroringStream::TDNMirroringStream(ITDNStreamProvider ^_target, ITDNStreamProvider ^_mirror)
   {
      target = _target;
      mirror = _mirror;

      stream = new MirroringStream(_target->GetStream(), _mirror->GetStream());
   }

   TDNMirroringStream::~TDNMirroringStream(void)
   {
   }

};
