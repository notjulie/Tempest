
#ifndef PISERIALSTREAM_H
#define PISERIALSTREAM_H

#include "TempestIO/AbstractTempestStream.h"

class PiSerialStream : public AbstractTempestStream
{
public:
   PiSerialStream(void);
   virtual ~PiSerialStream(void);

   virtual int  Read(void);
   virtual void Write(uint8_t b);

private:
   int fileStream;
};

#endif
