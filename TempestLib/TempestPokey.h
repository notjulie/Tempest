
#ifndef TEMPESTPOKEY_H
#define TEMPESTPOKEY_H

#include "Pokey/Pokey.h"

enum ButtonID {
	ONE_PLAYER_BUTTON
};


#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class TempestPokey1 : public Pokey
{
public:
	virtual uint8_t GetALLPOT(void) { return (uint8_t)(encoder & 0xF); }

	void MoveWheel(int delta) { encoder = (uint8_t)(encoder + delta); }

private:
	uint8_t encoder;
};

class TempestPokey2 : public Pokey
{
public:
	TempestPokey2(void)
	{
		buttons[ONE_PLAYER_BUTTON] = false;
	}

	virtual ~TempestPokey2(void) {}

	void SetButtonState(ButtonID button, bool pressed)
	{
		buttons[button] = pressed;
	}


	virtual uint8_t GetALLPOT(void) {
		//; BIT 0: D / E2 switch #2
		//; BIT 1: D / E2 switch #3
		//; BIT 2: D / E2 switch #4
		//; BIT 3: Fire Button
		//; BIT 4: Zapper Button
		//; BIT 5: Start Player 1 Button
		//; BIT 6: Start Player 2 Button
		//; BIT 7: Unused.
		uint8_t result = 0;
		if (buttons[ONE_PLAYER_BUTTON])
			result |= 0x20;
		return result;
	}

private:
	std::map<ButtonID, bool> buttons;
};

#pragma warning(pop)

#endif
