
#ifndef TEMPESTPOKEY_H
#define TEMPESTPOKEY_H

#include "TempestIO/AbstractTempestIO.h"
#include "Pokey.h"

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class TempestPokey1 : public Pokey
{
public:
	TempestPokey1(void)
		: Pokey(0)
	{
      lastReadEncoder = 0;
	}

	virtual uint8_t GetALLPOT(void) {
      // get the encoder
      uint8_t encoder = GetIO()->GetEncoder();

      // see how much it has changed since last time
      int8_t encoderChange = encoder - lastReadEncoder;

      // don't let it change too much in a single cycle... prevent wraparound
      if (encoderChange > 7)
         encoder = (uint8_t)(lastReadEncoder + 7);
      else if (encoderChange < - 7)
         encoder = (uint8_t)(lastReadEncoder - 7);

      // save and return
      lastReadEncoder = encoder;
      return encoder;
   }

private:
   uint8_t lastReadEncoder;
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
      if (buttons & TWO_PLAYER_BUTTON)
         result |= 0x40;
      if (buttons & FIRE_BUTTON)
			result |= 0x10;
		if (buttons & ZAPPER_BUTTON)
			result |= 0x08;
		return result;
	}
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
