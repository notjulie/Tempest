
#ifndef TEMPESTSTREAMPROTOCOL_H
#define TEMPESTSTREAMPROTOCOL_H

enum StreamOpCodes
{
	OP_6KHZ_TICK = 1,
   OP_BUTTON_LEDS = 2,
   OP_SOUND_WAVE = 3,
   OP_SOUND_FREQUENCY = 4,
   OP_SOUND_VOLUME = 5,
};

struct TempestInPacket {
   uint8_t  flags1;
};

enum TempestInPacketFlags1 {
};

#endif
