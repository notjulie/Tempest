
#ifndef SOUNDIOPACKET_H
#define SOUNDIOPACKET_H

#include "AbstractTempestIO.h"

class PacketStream;

class SoundIOPacketReader {
public:
   SoundIOPacketReader(const uint8_t *data, int length);

   bool GetButtonLED(ButtonFlag button);
   SoundChannelState GetInitialSoundChannelState(uint8_t channel);
   bool GetSoundCommand(uint8_t *delay, uint8_t *channel, SoundChannelState *state);

private:
   static const int ButtonsOffset = 0;
   static const int ChannelMaskOffset = 1;
   static const int ChannelDataOffset = 2;

public:
   static const int MaxPacketLength = 1000;
   static const int ClockCyclesPerTick = 250;

   // 31 is the maximum value without changing the current implementation; every command has
   // a delay associated with it and 31 is the maximum delay
   static const int TicksPerPacket = 31;
   static const int ClockCyclesPerPacket = ClockCyclesPerTick * TicksPerPacket;

private:
   const uint8_t *packet;
   int length;
   SoundChannelState initialChannelState[8];
   int offset;
};


class TempestInPacket {
public:
   bool ReadFromStream(PacketStream *stream);

public:
   uint8_t  flags1;
};

#endif

