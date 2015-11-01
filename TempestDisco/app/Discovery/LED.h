
#ifndef LED_H
#define LED_H

enum DiscoLED {
	DISCO_LED_RED,
	DISCO_LED_GREEN,
	DISCO_LED_BLUE,
	DISCO_LED_ORANGE
};

void InitializeLEDs(bool usePWM);
void LEDOn(DiscoLED led, bool on);
void LEDValue(DiscoLED led, uint16_t value);

#endif
