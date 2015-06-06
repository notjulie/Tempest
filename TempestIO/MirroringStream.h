
#ifndef MIRRORINGSTREAM_H
#define MIRRORINGSTREAM_H

#include "AbstractTempestStream.h"

class MirroringStream : public AbstractTempestStream
{
public:
   MirroringStream(AbstractTempestStream *_target, AbstractTempestStream *_mirror);

   virtual int  Read(void);
   virtual void Write(uint8_t b);

private:
   AbstractTempestStream *target;
   AbstractTempestStream *mirror;
};

#endif
