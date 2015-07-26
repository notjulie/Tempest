
#ifndef DISCORGB_H
#define DISCORGB_H

// high end of the range for red, green and blue values
#define RGB_MAX_VALUE 16

// Initializes the RGB outputs, including turning on the +/- 15V
// supply that XY also use
void InitializeRGB(void);

// Sets the RGB outputs, but as a one-shot of a given maximum duration;
// if ClearRGB is not called before the time elapses the WWDG forces them off.
// Note that the WWDG's timer is not precise... the time given here is a failsafe.
// The caller should clear the RGB outputs with ClearRGB at the proper time.
// Also, PulseRGB has a maximum value for the one-shot duration, so don't
// try passing in a value of 1000ms.
void PulseRGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t msFailsafeOffTime);

// Stops RGB output
void ClearRGB(void);

#endif
