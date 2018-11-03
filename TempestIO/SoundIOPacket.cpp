// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is the SoundIOPacketReader class that is used for reading
//    a sound IO packet from a serial stream.  In some implementations
//    of Tempest I use a separate processor for sound generation, and
//    this encapsulates some of the serial protocol used to
//    communicate between them.
// ====================================================================

#include "TempestIO.Headers.h"

#include "PacketStream.h"

#include "SoundIOPacket.h"


/// <summary>
/// Initializes a new instance of SoundIOPacketReader from a series of bytes received
/// from a serialized stream.
/// </summary>
SoundIOPacketReader::SoundIOPacketReader(const uint8_t *data, int length)
{
   // copy parameters
   this->packet = data;
   this->length = length;

   // decode the packet
   offset = ChannelDataOffset;
   uint8_t channelMask = packet[ChannelMaskOffset];
   for (int i = 0; i < 8; ++i)
   {
      if (channelMask & (1<<i))
      {
         ReadSoundChannelState(&initialChannelState[i]);
      }
      else
      {
         initialChannelState[i] = SoundChannelState();
      }
   }
}


/// <summary>
/// Gets the requested state of the given button's LED
/// </summary>
bool SoundIOPacketReader::GetButtonLED(ButtonFlag button)
{
   return (packet[ButtonsOffset] & button) != 0;
}


/// <summary>
/// Gets the initial sound channel state for the given channel; our packets contain a set of commands
/// that take place over a certain period, so for any channel that is making noise the receiver
/// needs to start with knowing the current state.  This way if a packet is missed due to a data
/// error the next packet will start from the correct state.
/// </summary>
SoundChannelState SoundIOPacketReader::GetInitialSoundChannelState(uint8_t channel)
{
   return initialChannelState[channel];
}

/// <summary>
/// Gets the next sound command; returns false to indicate end-of-packet
/// </summary>
bool SoundIOPacketReader::GetSoundCommand(uint8_t *delay, uint8_t *channel, SoundChannelState *state)
{
   if (offset >= length)
      return false;

   uint8_t b = packet[offset++];
   *delay = b & 0x1F;
   *channel = b>>5;
   ReadSoundChannelState(state);
   return true;
}

/// <summary>
/// Decodes a SoundChannelState structure from the current position in the packet
/// </summary>
void SoundIOPacketReader::ReadSoundChannelState(SoundChannelState *state)
{
   uint16_t encodedFrequency = packet[offset++] << 8;
   encodedFrequency += packet[offset++];
   state->SetChannelFrequency(DecodeFrequency(encodedFrequency));
   uint8_t volumeAndWaveform = packet[offset++];
   state->SetVolume(volumeAndWaveform & 0xF);
   state->SetWaveform((Waveform)(volumeAndWaveform >> 4));
}

/// <summary>
/// Unpacks a floating point frequency from a 16-bit float of our own invention;
/// this is used for compressing frequencies transmitted along the serial line.
/// <summary>
float SoundIOPacketReader::DecodeFrequency(uint16_t encodedFrequency)
{
   return (float)(encodedFrequency & 0xFFF) * (1 << (encodedFrequency >> 12)) / (1<<11);
}

/// <summary>
/// Packs a floating point frequency into a 16-bit float of our own invention;
/// this is used for compressing frequencies transmitted along the serial line.
/// <summary>
uint16_t SoundIOPacketReader::EncodeFrequency(float unencodedFrequency)
{
   // Our format is a 12-bit mantissa and a 4 bit exponent... the exponent
   // ranges from -11 to 4, so we have 12-bit precision for all frequencies in
   // the range 2Hz to 64KHz, and conveniently the maximum frequency in Tempest
   // is 64000, so we don't have to tweak anything there.

   // confine to our limits
   if (unencodedFrequency > 0xFFFF)
      unencodedFrequency = 0xFFFF;
   else if (unencodedFrequency < 0)
      unencodedFrequency = 0;

   // convert to a 32-bit int
   uint32_t freq32 = (uint32_t)(unencodedFrequency * (1<<11));

   int exponent = 0;
   while (exponent < 15 && freq32 > 0xFFF)
   {
      ++exponent;
      freq32 >>= 1;
   }

   return (uint16_t)freq32 | (exponent<<12);
}

TempestInPacket::TempestInPacket(void)
{
   memset(packet, 0, sizeof(packet));
}

bool TempestInPacket::ReadFromStream(PacketStream *stream)
{
   int bytesRead = stream->ReadPacket(packet, sizeof(packet));
   return bytesRead == sizeof(packet);
}

void TempestInPacket::WriteToStream(PacketStream *stream)
{
   stream->StartPacket();
   for (unsigned i = 0; i < sizeof(packet); ++i)
      stream->Write(packet[i]);
   stream->EndPacket();
}

bool TempestInPacket::operator!=(const TempestInPacket &packet)
{
   return memcmp(packet.packet, this->packet, PacketLength) != 0;
}
