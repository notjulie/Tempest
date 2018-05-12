
#ifndef ABSTRACTTEMPESTIO_H
#define ABSTRACTTEMPESTIO_H

#include "SoundChannel.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures
#endif

/// <summary>
/// Bit flag enumeration for button flags
/// </summary>
enum ButtonFlag {
   ONE_PLAYER_BUTTON = 1,
   FIRE_BUTTON = 2,
   ZAPPER_BUTTON = 4,
   TWO_PLAYER_BUTTON = 32
};

/// <summary>
/// Abstract class for a general arcade game control panel
/// </summary>
class AbstractArcadeGameControlPanel
{
public:
   virtual ~AbstractArcadeGameControlPanel(void) {}
   virtual uint8_t GetButtons(void) = 0;
   virtual uint8_t GetEncoder(void) = 0;
   virtual void SetButtonLED(ButtonFlag button, bool value) = 0;
};

/// <summary>
/// Simple implementation of AbstractArcadeGameControlPanel that just allows each
/// side to cooperatively write its output data and poll its input data.
/// </summary>
class SimpleArcadeGameControlPanel : public AbstractArcadeGameControlPanel
{
public:
   // base class implementation
   virtual uint8_t GetButtons(void) { return buttons; }
   virtual uint8_t GetEncoder(void) { return encoder; }
   virtual void SetButtonLED(ButtonFlag button, bool value) { if (value) buttonLEDs |= button; else buttonLEDs &= ~button; }

   // additional accessors
   bool GetPlayer1LEDState(void) const { return (buttonLEDs & ONE_PLAYER_BUTTON) != 0; }
   bool GetPlayer2LEDState(void) const { return (buttonLEDs & TWO_PLAYER_BUTTON) != 0; }
   void SetInputButtonState(ButtonFlag button, bool state) {
      if (state) buttons |= button;
      else buttons &= ~button;
   }
   void MoveSpinner(int offset) { encoder = (uint8_t)(encoder + offset); }
   
private:
   uint8_t buttons = 0;
   uint8_t buttonLEDs = 0;
   uint8_t encoder = 0;
};


/// <summary>
/// Abstract class that represents a Tempest's Pokey sound generators
/// </summary>
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


#ifdef _WIN32
#pragma warning(pop)
#endif

#endif
