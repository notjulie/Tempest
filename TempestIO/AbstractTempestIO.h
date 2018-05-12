
#ifndef ABSTRACTTEMPESTIO_H
#define ABSTRACTTEMPESTIO_H

#include "SoundChannel.h"

class VectorData;

enum ButtonFlag {
   ONE_PLAYER_BUTTON = 1,
   FIRE_BUTTON = 2,
   ZAPPER_BUTTON = 4,
   TWO_PLAYER_BUTTON = 32
};

class AbstractArcadeGameControlPanel
{
public:
   virtual ~AbstractArcadeGameControlPanel(void) {}
   virtual uint8_t GetButtons(void) = 0;
   virtual uint8_t GetEncoder(void) = 0;
   virtual void SetButtonLED(ButtonFlag button, bool value) = 0;
};

class AbstractTempestSoundOutput
{
public:
   virtual ~AbstractTempestSoundOutput(void) {}

	virtual void SetSoundChannelState(int channel, SoundChannelState state) = 0;
	virtual void SetTime(uint64_t clockCycles) = 0;

   void AllSoundOff(void) {
      SoundChannelState state;
      for (int i=0; i<8; ++i)
         SetSoundChannelState(i, state);
   }
};


#endif
