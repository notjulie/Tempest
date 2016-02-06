
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
   // process incoming data if we can
   while (readState != HavePacket)
   {
      int b = stream->Read();
      if (b < 0)
         return -1;

      switch (readState)
      {
      case ReadIdle:
         if (b == START_OF_PACKET)
         {
            readState = InPacket;
            incomingPacketLength = 0;
         }
         break;

      case InPacket:
         switch (b)
         {
         case END_OF_PACKET:
            if (VerifyIncomingPacket())
            {
               readState = HavePacket;
               incomingPacketLength -= 2;
               incomingPacketReadIndex = 0;
            }
            else
            {
               readState = ReadIdle;
            }
            break;

         case ESCAPE:
            readState = Escape;
            break;

         default:
            incomingPacket[incomingPacketLength++] = b;
            if (incomingPacketLength >= sizeof(incomingPacket))
               readState = ReadIdle;
            break;
         }
         break;

      case Escape:
         b += ESCAPE;
         if (b <= 255)
         {
            incomingPacket[incomingPacketLength++] = b;
            if (incomingPacketLength >= sizeof(incomingPacket))
               readState = ReadIdle;
         }
         else
         {
            readState = ReadIdle;
         }
         break;
      }
   }

   // we're in HavePacket state
   uint8_t result = incomingPacket[incomingPacketReadIndex++];
   if (incomingPacketReadIndex >= incomingPacketLength)
      readState = ReadIdle;
   return result;
}

bool PacketStream::VerifyIncomingPacket(void)
{
   // it must be greater than two bytes long: length and checksum and one data byte
   if (incomingPacketLength < 3)
      return false;
   if (incomingPacket[incomingPacketLength - 1] != (uint8_t)(incomingPacketLength-1))
      return false;

   uint8_t checksum = 0;
   for (int i = 0; i < incomingPacketLength - 2; ++i)
      checksum += incomingPacket[i];
   if (incomingPacket[incomingPacketLength - 2] != (uint8_t)checksum)
      return false;

   return true;
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

   ++outgoingPacketLength;
   outgoingPacketCheckSum += b;
}


void PacketStream::StartPacket(void)
{
   stream->Write(START_OF_PACKET);
   outgoingPacketCheckSum = 0;
   outgoingPacketLength = 0;
}

void PacketStream::EndPacket(void)
{
   // write the length and checksum with escaping
   Write(outgoingPacketCheckSum);
   Write(outgoingPacketLength);

   // write the end of packet without escaping
   stream->Write(END_OF_PACKET);
}

