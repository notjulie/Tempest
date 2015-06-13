
#include "stdafx.h"
#include "MirroringStream.h"


MirroringStream::MirroringStream(AbstractTempestStream *_target, AbstractTempestStream *_mirror)
{
   target = _target;
   mirror = _mirror;
}

int MirroringStream::Read(void)
{
   // empty the mirroring stream
   while (mirror->Read() >= 0)
      continue;

   // return what we read from the target
   return target->Read();
}

void MirroringStream::Write(uint8_t b)
{
   // write to both the target and the mirror
   target->Write(b);
   mirror->Write(b);
}
