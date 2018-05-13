
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
         initialChannelState[i].SetFrequency(packet[offset++]);
         uint8_t volumeAndWaveform = packet[offset++];
         initialChannelState[i].SetVolume(volumeAndWaveform & 0xF);
         initialChannelState[i].SetWaveform((Waveform)(volumeAndWaveform >> 4));
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
   state->SetFrequency(packet[offset++]);
   uint8_t volumeAndWaveform = packet[offset++];
   state->SetVolume(volumeAndWaveform & 0xF);
   state->SetWaveform((Waveform)(volumeAndWaveform >> 4));
   return true;
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
