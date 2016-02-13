
#include "stdafx.h"
#include "SoundChannelState.h"

SoundChannelState::SoundChannelState(void)
{
   volume = 0;
   waveform = 0;
   frequency = 0;
}

uint8_t SoundChannelState::GetVolumeAndWaveform(void) const
{
   return volume | (waveform << 4);
}


void SoundChannelState::SetVolumeAndWaveform(uint8_t volumeAndWaveform)
{
   volume = volumeAndWaveform & 0x0F;
   waveform = volumeAndWaveform >> 4;
}

bool SoundChannelState::operator==(const SoundChannelState &state) const
{
   // if they both have zero volume the are equal
   if (state.volume == 0 && this->volume == 0)
      return true;

   // else everything has to match
   return
      state.frequency == this->frequency &&
      state.volume == this->volume &&
      state.waveform == this->waveform;
}

bool SoundChannelState::operator!=(const SoundChannelState &state) const
{
   return !operator==(state);
}

