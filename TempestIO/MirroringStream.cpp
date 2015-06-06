
#include "stdafx.h"
#include "MirroringStream.h"


MirroringStream::MirroringStream(AbstractTempestStream *_target, AbstractTempestStream *_mirror)
{
   target = _target;
   mirror = _mirror;
}

int MirroringStream::Read(void)
{
   // just read from the target... the mirror is not involved
   return target->Read();
}

void MirroringStream::Write(uint8_t b)
{
   // write to both the target and the mirror
   target->Write(b);
   mirror->Write(b);
}
