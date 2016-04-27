
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
   static const unsigned ClockCyclesPerPacket = ClockCyclesPerTick * TicksPerPacket;

   // we try to make sure to send the control panel info 50 times a second even if
   // it hasn't changed
   static const int MaxResponsePacketPeriod = 1500000 / 50;

private:
   const uint8_t *packet;
   int length;
   SoundChannelState initialChannelState[8];
   int offset;
};


class TempestInPacket {
public:
   TempestInPacket(void);

   uint8_t GetButtons(void) const { return packet[ButtonsOffset]; }
   uint8_t GetEncoder(void) const { return packet[EncoderOffset]; }

   void SetButtons(uint8_t buttons) { packet[ButtonsOffset] = buttons; }
   void SetEncoder(uint8_t encoder) { packet[EncoderOffset] = encoder; }

   bool ReadFromStream(PacketStream *stream);
   void WriteToStream(PacketStream *stream);

   bool operator!=(const TempestInPacket &packet);

private:
   static const int ButtonsOffset = 0;
   static const int EncoderOffset = 1;
   static const int PacketLength = 2;

private:
   uint8_t  packet[PacketLength];
};

#endif

