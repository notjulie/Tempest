
#include "stdafx.h"

#include "PacketStream.h"

#include "SoundIOPacket.h"


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


bool SoundIOPacketReader::GetButtonLED(ButtonFlag button)
{
   return (packet[ButtonsOffset] & button) != 0;
}

SoundChannelState SoundIOPacketReader::GetInitialSoundChannelState(uint8_t channel)
{
   return initialChannelState[channel];
}

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

void SoundIOPacketReader::ReadSoundChannelState(SoundChannelState *state)
{
   uint16_t encodedFrequency = packet[offset++] << 8;
   encodedFrequency += packet[offset++];
   state->SetChannelFrequency(DecodeFrequency(encodedFrequency));
   uint8_t volumeAndWaveform = packet[offset++];
   state->SetVolume(volumeAndWaveform & 0xF);
   state->SetWaveform((Waveform)(volumeAndWaveform >> 4));
}

float SoundIOPacketReader::DecodeFrequency(uint16_t encodedFrequency)
{
   return (float)(encodedFrequency & 0x1FFF) / (1 << (encodedFrequency >> 13));
}

uint16_t SoundIOPacketReader::EncodeFrequency(float unencodedFrequency)
{
   // our max fundamental is 0x1FFF, ~8KHz
   if (unencodedFrequency > 0x1FFF)
      unencodedFrequency = 0x1FFF;

   int exponent = 0;
   while (exponent < 7 && unencodedFrequency <= 0xFFF)
   {
      ++exponent;
      unencodedFrequency *= 2;
   }

   return (uint16_t)unencodedFrequency | (exponent<<13);
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
