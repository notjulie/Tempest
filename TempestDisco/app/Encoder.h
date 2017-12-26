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

class EncoderInput
{
public:
	EncoderInput(int sampleFrequency);

	void AddSample(int value);
	bool IsHigh(void) const { return isHigh; }

private:
	bool isHigh;
	int lowPassConstant;
	int highPassConstant;
	int lowPassCapacitor;
	int highPassCapacitor;
};

class Encoder
{
public:
	Encoder(int sampleFrequency);

	void AddSample(int a, int b);
	uint16_t GetValue(void) const { return currentValue; }

private:
	EncoderInput input1;
	EncoderInput input2;
	int phaseAccumulator;
	uint16_t currentValue;
};

#endif
