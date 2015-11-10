
#ifndef PISERIALSTREAM_H
#define PISERIALSTREAM_H

#include "TempestIO/AbstractTempestStream.h"

class PiSerialStream : public AbstractTempestStream
{
public:
   virtual int  Read(void) { return -1; }
   virtual void Write(uint8_t b) {}
};

#endif
