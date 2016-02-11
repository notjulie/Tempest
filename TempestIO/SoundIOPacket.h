
#ifndef SOUNDIOPACKET_H
#define SOUNDIOPACKET_H

#include "AbstractTempestIO.h"

class SoundIOPacketReader {
public:
   SoundIOPacketReader(const uint8_t *data, int length);

   bool GetButtonLED(ButtonFlag button);
   uint8_t GetSoundChannelFrequency(uint8_t channel);
   uint8_t GetSoundChannelVolume(uint8_t channel);
   uint8_t GetSoundChannelWaveform(uint8_t channel);

private:
   static const int ButtonsOffset = 0;
   static const int ChannelMaskOffset = 1;
   static const int ChannelDataOffset = 2;

public:
   static const int MaxPacketLength = 1000;
   static const int ClockCyclesPerPacket = 250;

private:
   const uint8_t *packet;
   int length;
   uint8_t frequencies[8];
   uint8_t volumes[8];
   uint8_t waveforms[8];
};

#endif

