
#ifndef TEMPESTMEMORYSTREAM_H
#define TEMPESTMEMORYSTREAM_H

#include "AbstractTempestStream.h"
#include "TempestStreamProtocol.h"

class TempestMemoryStream : public AbstractTempestInStream, public AbstractTempestOutStream
{
public:
	TempestMemoryStream(void);

   virtual int  Read(void);
   virtual void Write(uint8_t b);

   virtual bool Read(TempestInPacket *packet);
   virtual void Write(TempestInPacket packet);

private:
	uint8_t buffer[10000];
	int bufferIn;
	int bufferOut;

   TempestInPacket packetBuffer[500];
   int packetBufferIn;
   int packetBufferOut;
};


#endif
