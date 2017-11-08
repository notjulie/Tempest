// ===============================================================
// Encoder.h
//
//   Tempest encoder wheel class header
//     Author: Randy Rasmussen
//     Copyright: None
//     Warranty: None
//
// ===============================================================

#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

class Encoder
{
public:
	Encoder(void);

	void AddSample(int a, int b);
	uint16_t GetValue(void) const { return currentValue; }

private:
	int aThreshold;
	int bThreshold;
	int phase;
	int phaseAccumulator;
	uint16_t currentValue;
};

#endif
