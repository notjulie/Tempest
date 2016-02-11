
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
         frequencies[i] = packet[offset++];
         uint8_t b = packet[offset++];
         waveforms[i] = b & 0x0F;
         volumes[i] = b >> 4;
      }
      else
      {
         frequencies[i] = 0;
         waveforms[i] = 0;
         volumes[i] = 0;
      }
   }
}


bool SoundIOPacketReader::GetButtonLED(ButtonFlag button)
{
   return (packet[ButtonsOffset] & button) != 0;
}

uint8_t SoundIOPacketReader::GetSoundChannelFrequency(uint8_t channel)
{
   return frequencies[channel];
}

uint8_t SoundIOPacketReader::GetSoundChannelVolume(uint8_t channel)
{
   return volumes[channel];
}

uint8_t SoundIOPacketReader::GetSoundChannelWaveform(uint8_t channel)
{
   return waveforms[channel];
}

