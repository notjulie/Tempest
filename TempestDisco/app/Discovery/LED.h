
#ifndef LED_H
#define LED_H

void InitializeLEDs(bool usePWM);
void LEDRedOn(bool on);
void LEDGreenOn(bool on);
void LEDBlueOn(bool on);
void LEDOrangeOn(bool on);

void LEDRedValue(uint16_t value);
void LEDGreenValue(uint16_t value);
void LEDBlueValue(uint16_t value);
void LEDOrangeValue(uint16_t value);

#endif
