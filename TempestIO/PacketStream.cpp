
#include "stdafx.h"

#include "AbstractTempestStream.h"

#include "PacketStream.h"

static const uint8_t START_OF_PACKET = 0xFF;
static const uint8_t END_OF_PACKET = 0xFE;
static const uint8_t ESCAPE = 0xFD;



PacketStream::PacketStream(AbstractTempestStream *stream)
{
   // save parameters
   this->stream = stream;

   // clear
   readState = ReadIdle;
}

int PacketStream::Read(void)
{
   for (;;)
   {
      int b = stream->Read();
      if (b < 0)
         return b;

      switch (readState)
      {
      case ReadIdle:
         if (b == START_OF_PACKET)
            readState = InPacket;
         break;

      case InPacket:
         switch (b)
         {
         case END_OF_PACKET:
            readState = ReadIdle;
            break;

         case ESCAPE:
            readState = Escape;
            break;

         default:
            return b;
         }
         break;

      case Escape:
         b += ESCAPE;
         if (b <= 255)
         {
            readState = InPacket;
            return b;
         }
         readState = ReadIdle;
         break;
      }
   }
}

void PacketStream::Write(uint8_t b)
{
   if (b >= ESCAPE)
   {
      stream->Write(ESCAPE);
      stream->Write(b - ESCAPE);
   }
   else
   {
      stream->Write(b);
   }
}


void PacketStream::StartPacket(void)
{
   stream->Write(START_OF_PACKET);
}

void PacketStream::EndPacket(void)
{
   stream->Write(END_OF_PACKET);
}
