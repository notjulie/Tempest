
#include "stdafx.h"
#include "SoundIOPacket.h"


SoundIOPacketReader::SoundIOPacketReader(const uint8_t *data, int length)
{
   // copy parameters
   this->packet = data;
   this->length = length;

   // decode the packet
   int offset = ChannelDataOffset;
   uint8_t channelMask = packet[ChannelMaskOffset];
   for (int i = 0; i < 8; ++i)
   {
      if (channelMask & (1<<i))
      {
         channelState[i].SetFrequency(packet[offset++]);
         channelState[i].SetVolumeAndWaveform(packet[offset++]);
      }
      else
      {
         channelState[i] = SoundChannelState();
      }
   }
}


bool SoundIOPacketReader::GetButtonLED(ButtonFlag button)
{
   return (packet[ButtonsOffset] & button) != 0;
}

SoundChannelState SoundIOPacketReader::GetSoundChannelState(uint8_t channel)
{
   return channelState[channel];
}

