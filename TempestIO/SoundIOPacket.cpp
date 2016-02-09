
#include "stdafx.h"
#include "SoundIOPacket.h"


SoundIOPacket::SoundIOPacket(void)
{
   for (int i = 0; i < 8; ++i)
      SetSoundChannelVolume(i, 0);
   SetButtonLED(ONE_PLAYER_BUTTON, false);
   SetButtonLED(TWO_PLAYER_BUTTON, false);
}

SoundIOPacket::SoundIOPacket(const uint8_t *data)
{
   for (int i = 0; i < PacketLength; ++i)
      packet[i] = data[i];
}


bool SoundIOPacket::GetButtonLED(ButtonFlag button)
{
   return (packet[ButtonsOffset] & button) != 0;
}

uint8_t SoundIOPacket::GetSoundChannelFrequency(uint8_t channel)
{
   return packet[FrequencyOffset + channel];
}

uint8_t SoundIOPacket::GetSoundChannelVolume(uint8_t channel)
{
   return packet[WaveformVolumeOffset + channel] & 0x0F;
}

uint8_t SoundIOPacket::GetSoundChannelWaveform(uint8_t channel)
{
   return (packet[WaveformVolumeOffset + channel] & 0xF0) >> 4;
}

void SoundIOPacket::SetButtonLED(ButtonFlag button, bool value)
{
   if (value)
      packet[ButtonsOffset] |= button;
   else
      packet[ButtonsOffset] &= ~button;
}

void SoundIOPacket::SetSoundChannelFrequency(uint8_t channel, uint8_t frequency)
{
   packet[FrequencyOffset + channel] = frequency;
}

void SoundIOPacket::SetSoundChannelVolume(uint8_t channel, uint8_t volume)
{
   uint8_t b = packet[WaveformVolumeOffset + channel];
   b &= 0xF0;
   b |= (volume & 0x0F);
   packet[WaveformVolumeOffset + channel] = b;
}

void SoundIOPacket::SetSoundChannelWaveform(uint8_t channel, uint8_t waveform)
{
   uint8_t b = packet[WaveformVolumeOffset + channel];
   b &= 0x0F;
   b |= ((waveform<<4) & 0xF0);
   packet[WaveformVolumeOffset + channel] = b;
}
