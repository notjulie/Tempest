
#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

class Encoder
{
public:
	Encoder(void);

	void AddSample(int input1, int input2);
	uint16_t GetValue(void) const { return currentValue; }

private:
	bool isInitialized;
	bool wasInput1Higher;
	bool wasHigh;
	int lowCrossingLevel;
	int highCrossingLevel;
	uint16_t currentValue;
};

#endif
