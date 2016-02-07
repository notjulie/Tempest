
#ifndef SOUNDIOPACKET_H
#define SOUNDIOPACKET_H

#include "AbstractTempestIO.h"

class SoundIOPacket {
public:
   SoundIOPacket(void);
   SoundIOPacket(const uint8_t *data);

   bool GetButtonLED(ButtonFlag button);
   uint8_t GetElapsedTicks(void) const { return packet[TicksOffset]; }
   uint8_t GetSoundChannelFrequency(uint8_t channel);
   uint8_t GetSoundChannelVolume(uint8_t channel);
   uint8_t GetSoundChannelWaveform(uint8_t channel);

   void SetButtonLED(ButtonFlag button, bool value);
   void SetElapsedTicks(uint8_t elapsedTicks){ packet[TicksOffset] = elapsedTicks; }
   void SetSoundChannelFrequency(uint8_t channel, uint8_t frequency);
   void SetSoundChannelVolume(uint8_t channel, uint8_t volume);
   void SetSoundChannelWaveform(uint8_t channel, uint8_t waveform);

   const uint8_t *GetPacketData(void) const { return packet; }
   int GetLength(void) const { return PacketLength; }

private:
   static const int FrequencyOffset = 0;
   static const int WaveformVolumeOffset = 8;
   static const int TicksOffset = 16;
   static const int ButtonsOffset = 17;

public:
   static const int PacketLength = 18;

private:
   uint8_t packet[PacketLength];
};

#endif

