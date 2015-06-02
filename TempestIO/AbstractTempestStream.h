
#ifndef ABSTRACTTEMPESTSTREAM_H
#define ABSTRACTTEMPESTSTREAM_H

struct TempestInPacket;

class AbstractTempestOutStream
{
public:
   virtual bool Read(TempestInPacket *packet) = 0;
   virtual void Write(uint8_t b) = 0;
};

class AbstractTempestInStream
{
public:
   virtual int  Read(void) = 0;
   virtual void Write(TempestInPacket packet) = 0;
};

#endif
