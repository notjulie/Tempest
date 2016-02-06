
#include "stdafx.h"

#include "AbstractTempestStream.h"

#include "PacketStream.h"

PacketStream::PacketStream(AbstractTempestStream *stream)
{
   this->stream = stream;
}

int PacketStream::Read(void)
{
   return stream->Read();
}

void PacketStream::Write(uint8_t b)
{
   stream->Write(b);
}


void PacketStream::StartPacket(void)
{

}

void PacketStream::EndPacket(void)
{

}
