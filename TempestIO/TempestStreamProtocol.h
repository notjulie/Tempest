
#ifndef TEMPESTSTREAMPROTOCOL_H
#define TEMPESTSTREAMPROTOCOL_H

enum StreamOpCodes
{
	OP_VECTOR_RESET = 0,
	OP_6KHZ_TICK = 1,
	OP_WRITE_VECTOR_RAM = 2,
   OP_SOUND_WAVE = 3,
   OP_SOUND_FREQUENCY = 4,
   OP_SOUND_VOLUME = 5,
   OP_VECTOR_GO = 6
};

struct TempestInPacket {
   uint8_t  flags1;
};

enum TempestInPacketFlags1 {
   FLAG_VECTOR_HALT = 1
};

#endif
