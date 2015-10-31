
#ifndef TEMPESTPOKEY_H
#define TEMPESTPOKEY_H

#include "../TempestIO/AbstractTempestIO.h"
#include "Pokey.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class TempestPokey1 : public Pokey
{
public:
	TempestPokey1(void)
		: Pokey(0)
	{
	}

	virtual uint8_t GetALLPOT(void) { return GetIO()->GetEncoder(); }
};

class TempestPokey2 : public Pokey
{
public:
	TempestPokey2(void)
		: Pokey(4)
	{
	}

	virtual ~TempestPokey2(void) {}

	virtual uint8_t GetALLPOT(void) {
		//; BIT 0: D / E2 switch #2
		//; BIT 1: D / E2 switch #3
		//; BIT 2: D / E2 switch #4
		//; BIT 3: Zapper Button
		//; BIT 4: Fire Button
		//; BIT 5: Start Player 1 Button
		//; BIT 6: Start Player 2 Button
		//; BIT 7: Unused.
		uint8_t result = 0;
      uint8_t buttons = GetIO()->GetButtons();
		if (buttons & ONE_PLAYER_BUTTON)
			result |= 0x20;
		if (buttons & FIRE_BUTTON)
			result |= 0x10;
		if (buttons & ZAPPER_BUTTON)
			result |= 0x08;
		return result;
	}
};

#pragma warning(pop)

#endif
