// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//		This has support for the LED's on the STM32F4 Discovery board,
//		which are used for general diagnostics.  The LEDs are driven by
//    PWM, so they are really flexible in how they can be used, more
//    than just on/off.
// ====================================================================

#ifndef LED_H
#define LED_H

/// <summary>
/// Enumeration of the LEDs
/// </summary>
enum DiscoLED {
	DISCO_LED_RED,
	DISCO_LED_GREEN,
	DISCO_LED_BLUE,
	DISCO_LED_ORANGE
};

/// <summary>
/// Initialize the LEDs; called once on startup
/// </summary>
void InitializeLEDs(void);

void LEDOn(DiscoLED led, bool on);
void LEDValue(DiscoLED led, uint16_t value);

#endif
