
#ifndef SOUNDCHANNELSTATE_H
#define SOUNDCHANNELSTATE_H


class SoundChannelState
{
public:
   SoundChannelState(void);

   uint8_t GetFrequency(void) const { return frequency; }
   uint8_t GetVolume(void) const { return volume; }
   uint8_t GetVolumeAndWaveform(void) const;
   uint8_t GetWaveform(void) const { return waveform; }
   void SetFrequency(uint8_t frequency) { this->frequency = frequency; }
   void SetVolumeAndWaveform(uint8_t volumeAndWaveform);

   bool operator==(const SoundChannelState &state) const;
   bool operator!=(const SoundChannelState &state) const;

private:
   uint8_t frequency;
   uint8_t volume;
   uint8_t waveform;
};

#endif
