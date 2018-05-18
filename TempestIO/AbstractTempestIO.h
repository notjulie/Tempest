
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
   ONE_PLAYER_BUTTON = (1 << 0),
   FIRE_BUTTON = (1<<1),
   ZAPPER_BUTTON = (1<<2),
   TWO_PLAYER_BUTTON = (1<<3),
   ROTATE_LEFT_BUTTON = (1<<4),
   ROTATE_RIGHT_BUTTON = (1<<5)
};

/// <summary>
/// Abstract class for a general arcade game control panel reader; something that
/// a user of the control panel might want.
/// </summary>
class AbstractArcadeGameControlPanelReader
{
public:
   virtual ~AbstractArcadeGameControlPanelReader(void) {}
   virtual uint8_t GetButtons(void) = 0;
   virtual uint8_t GetEncoder(void) = 0;
   virtual void SetButtonLED(ButtonFlag button, bool value) = 0;
};

/// <summary>
/// Abstract class for a general arcade game control panel writer; something that
/// pushes the state of the control panel to someone else.
/// </summary>
class AbstractArcadeGameControlPanelWriter
{
public:
   virtual ~AbstractArcadeGameControlPanelWriter(void) {}
   virtual void SetButtonState(ButtonFlag button, bool state) = 0;
   virtual void MoveEncoder(int8_t distance) = 0;
   virtual bool GetButtonLED(ButtonFlag button) const = 0;
};

/// <summary>
/// Simple implementation of AbstractArcadeGameControlPanel that just allows each
/// side to cooperatively write its output data and poll its input data.
/// </summary>
class SimpleArcadeGameControlPanel :
   public AbstractArcadeGameControlPanelReader,
   public AbstractArcadeGameControlPanelWriter
{
public:
   SimpleArcadeGameControlPanel(void) {
      buttons = 0;
      buttonLEDs = 0;
      encoder = 0;
   }
   // AbstractArcadeGameControlPanelReader implementation
   virtual uint8_t GetButtons(void) { return buttons; }
   virtual uint8_t GetEncoder(void) { return encoder; }
   virtual void SetButtonLED(ButtonFlag button, bool value) { if (value) buttonLEDs |= button; else buttonLEDs &= ~button; }

   // AbstractArcadeGameControlPanelWriter implementation
   virtual void SetButtonState(ButtonFlag button, bool state) {
      if (state) buttons |= button;
      else buttons &= ~button;
   }
   virtual void MoveEncoder(int8_t distance) { encoder = (uint8_t)(encoder + distance); }
   virtual bool GetButtonLED(ButtonFlag button) const { return (buttonLEDs & button) != 0; }

private:
   uint8_t buttons;
   uint8_t buttonLEDs;
   uint8_t encoder;
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
