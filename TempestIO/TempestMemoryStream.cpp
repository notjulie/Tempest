
#include "stdafx.h"

#include "../TempestCPU/TempestException.h"

#include "TempestMemoryStream.h"

TempestMemoryStream::TempestMemoryStream(void)
{
	bufferIn = 0;
	bufferOut = 0;
   packetBufferIn = 0;
   packetBufferOut = 0;
}

void TempestMemoryStream::Write(uint8_t b)
{
	// figure out what our index will be after appending the byte
	int newBufferIn = bufferIn + 1;
	if (newBufferIn >= sizeof(buffer))
		newBufferIn = 0;
	if (newBufferIn == bufferOut)
		throw TempestException("TempestMemoryStream::Write: buffer full");

	buffer[bufferIn] = b;
	bufferIn = newBufferIn;
}

int TempestMemoryStream::Read(void)
{
   // never mind if there's nothing
   if (bufferIn == bufferOut)
      return -1;

   // else just return the next
   int newBufferOut = bufferOut + 1;
   if (newBufferOut >= sizeof(buffer))
      newBufferOut = 0;
   int result = buffer[bufferOut];
   bufferOut = newBufferOut;
   return result;
}


bool TempestMemoryStream::Read(TempestInPacket *packet)
{
   // never mind if there's nothing
   if (packetBufferIn == packetBufferOut)
      return false;

   // else just return the next
   int newPacketBufferOut = packetBufferOut + 1;
   if (newPacketBufferOut >= sizeof(packetBuffer) / sizeof(packetBuffer[0]))
      newPacketBufferOut = 0;
   *packet = packetBuffer[packetBufferOut];
   packetBufferOut = newPacketBufferOut;
   return true;
}

void TempestMemoryStream::Write(TempestInPacket packet)
{
   // figure out what our index will be after appending the byte
   int newPacketBufferIn = packetBufferIn + 1;
   if (newPacketBufferIn >= sizeof(packetBuffer) / sizeof(packetBuffer[0]))
      newPacketBufferIn = 0;
   
   if (newPacketBufferIn == packetBufferOut)
      throw TempestException("TempestMemoryStream::Write: packet buffer full");

   packetBuffer[packetBufferIn] = packet;
   packetBufferIn = newPacketBufferIn;
}
