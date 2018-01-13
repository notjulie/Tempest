// ===============================================================
// Encoder.cpp
//
//   Tempest encoder wheel class implementation
//     Author: Randy Rasmussen
//     Copyright: None
//     Warranty: None
//
// ===============================================================

#include <math.h>
#include "Encoder.h"


// =============================================================
// =============================================================
// =============================================================
//              class EncoderInput
// =============================================================
// =============================================================
// =============================================================

/// <summary>
/// Initializes a new instance of class EncoderInput
/// </summary>
EncoderInput::EncoderInput(int sampleFrequency)
{
	// initialize
	isHigh = false;
	lowPassCapacitor = 0;
	highPassCapacitor = 0;

	// calculate our sample period
	double secondsPerCycle = 1.0 / sampleFrequency;

	// calculate the constant we use in our lowpass filter... we just want the fractional
	// change in the capacitor voltage over the course of one sample period
	double lowPassTimeConstantMS = 0.33; // 3.3K times 0.1uF is what the schematic says
	double dropPerCycle = 1.0 - exp(-secondsPerCycle / (lowPassTimeConstantMS / 1000));
	lowPassConstant = (int)(1000 * dropPerCycle);

	// similarly calculate the constant we use in our highpass filter
	double highPassTimeConstantMS = 10.0; // 10K times 1uF is what the schematic
	dropPerCycle = 1.0 - exp(-secondsPerCycle / (highPassTimeConstantMS / 1000));
	highPassConstant = (int)(1000 * dropPerCycle);
}

/// <summary>
/// Processes a sample
/// </summary>
void EncoderInput::AddSample(int value)
{
	// ==== THEORY OF OPERATION ====
	// This is actually a simulation of the original Tempest circuit, which
	// feeds the analog inputs into a Schmitt-triggered inverter.  But it also
	// does the following:
	//    - biases the circuit so the Schmitt-trigger's threshold is effecively
	//      zero (plus or minus the hysteresis)
	//    - lowpasses the input, presumably to reduce noise
	//    - highpasses the input, to remove DC
	//
	// Following all of that it is a simple matter of transitioning to high if
	// the filtered output is greater than the hysteresis, and transitioning low
	// if it is less than the negative of the hysteresis.

	// do the lowpass
	lowPassCapacitor += lowPassConstant * (value - lowPassCapacitor) / 1000;

	// do the highpass
	highPassCapacitor += highPassConstant * (lowPassCapacitor - highPassCapacitor) / 1000;

	// get the highpass output
	double highpassOutput = lowPassCapacitor - highPassCapacitor;

	// adjust our value accordingly
	int hysteresis = 3000;
	if (highpassOutput < -hysteresis)
		isHigh = false;
	else if (highpassOutput > hysteresis)
		isHigh = true;
}




// =============================================================
// =============================================================
// =============================================================
//              class Encoder
// =============================================================
// =============================================================
// =============================================================

Encoder::Encoder(int sampleFrequency)
	:
		input1(sampleFrequency),
		input2(sampleFrequency)
{
	phaseAccumulator = 0;
	currentValue = 0;
}


// ==================================================================
// AddSample(a, b)
//    a: one encoder input, analog, range 0 - 32K
//    b: the other encoder input, analog, range 0 - 32K
//
// Processes a sample of the encoder inputs, and updates the
// output accordingly.
// ==================================================================
void Encoder::AddSample(int a, int b)
{
	// get the previous values of our inputs
	uint8_t transition = 0;
	if (input1.IsHigh())
		transition |= 0x10;
	if (input2.IsHigh())
		transition |= 0x20;

	// update the inputs
	input1.AddSample(a);
	input2.AddSample(b);

	// get the current values
	if (input1.IsHigh())
		transition |= 0x01;
	if (input2.IsHigh())
		transition |= 0x02;

	// our transition byte is set up for handy readability, such that
	// 0x12 means that our inputs were 0x1 beforehand and 0x2 after
	switch (transition)
	{
	case 0x01:
	case 0x13:
	case 0x32:
	case 0x20:
		// these are all the positive moving transitions
		++phaseAccumulator;
		break;

	case 0x02:
	case 0x23:
	case 0x31:
	case 0x10:
		// these are all the negative moving transitions
		--phaseAccumulator;
		break;
		break;
	}

	// Update our output... we change our output on every half cycle, thus
	// every time we accumulate two phase changes in the same direction.
	// This is also further debouncing beyond just the hysteresis, too.
	if (phaseAccumulator >= 2)
	{
		phaseAccumulator -= 2;
		++currentValue;
	}
	else if (phaseAccumulator <= -2)
	{
		phaseAccumulator += 2;
		--currentValue;
	}
}


