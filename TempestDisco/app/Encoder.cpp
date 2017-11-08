// ===============================================================
// Encoder.cpp
//
//   Tempest encoder wheel class implementation
//     Author: Randy Rasmussen
//     Copyright: None
//     Warranty: None
//
// ===============================================================


#include "Encoder.h"


Encoder::Encoder(void)
{
	phaseAccumulator = 0;
	currentValue = 0;
	phase = 0x00A;
	aThreshold = bThreshold = 0;
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
	// define the margin between a and b for which they are considered
	// within a noise/hysteresis margin of being equal
	const int SIGNAL_CROSSING_MARGIN = 1000;

	// define our individual signal hysteresis
	const int HYSTERESIS = 2000;

	// Theory of operation: this is a regular old encoder, with two signals
	// out of phase by 90 degrees so that the changes in phase tell us
	// which direction the knob is spinning.  The challenges here are:
	//
	//   - The inputs are analog, and there is no clear threshold between
	//     high and low.  Moreover, the two channels are often of different
	//     magnitudes, and the magnitude can tend to drift.
	//
	//	  - Noise... the signals are pretty noisy, and so it's very easy
	//     to get deceived into thinking the spinner is moving when it isn't
	//
	// Normally one thinks of such an encoder in 4 phases, low-low, low-high,
	// high-low, high-high.  That's pretty good, but I have seen cases where it
	// jumps a phase, especially when control panel vibrations are present.
	// As a result I added two more phases, by noting the crossing point of
	// the two inputs.  Those phases are:
	//
	//  0x00A:  a low, b low, a greater than b
	//  0x00B:  a low, b low, b greater than a
	//  0x01B:  a low, b high, (b greater than a, implied)
	//  0x10A:  a high, b low, (a greater than b, implied)
	//  0x11A:  a high, b high, a greater than b
	//  0x11B:  a high, b high, b greater than a
	//
	// The addition of the extra two phases makes it nearly impossible for a noisy
	// signal to traverse the circle... in general it will stop or reverse its
	// direction.


	// branch according to phase
	switch (phase)
	{
	case 0x00A:	// both signals low, a greater than b
		// check to see if the signals have crossed
		if (b > a + SIGNAL_CROSSING_MARGIN)
		{
			aThreshold = a + HYSTERESIS;
			bThreshold = b + HYSTERESIS;
			--phaseAccumulator;
			phase = 0x00B;
			break;
		}

		// check to see if a has changed from low to high
		if (a > aThreshold)
		{
			++phaseAccumulator;
			phase = 0x10A;
		}
		break;

	case 0x00B:	// both signals low, b greater than a
		// check to see if the signals have crossed
		if (a > b + SIGNAL_CROSSING_MARGIN)
		{
			aThreshold = a + HYSTERESIS;
			bThreshold = b + HYSTERESIS;
			++phaseAccumulator;
			phase = 0x00A;
			break;
		}

		// check to see if b has changed from low to high
		if (b > bThreshold)
		{
			--phaseAccumulator;
			phase = 0x01B;
		}
		break;

	case 0x01B:	// a low, b high
		if (a > aThreshold)
		{
			--phaseAccumulator;
			phase = 0x11B;
		}
		else if (b < bThreshold)
		{
			++phaseAccumulator;
			phase = 0x00B;
		}
		break;

	case 0x10A: // a high, b low
		if (a < aThreshold)
		{
			--phaseAccumulator;
			phase = 0x00A;
		}
		else if (b > bThreshold)
		{
			++phaseAccumulator;
			phase = 0x11A;
		}
		break;

	case 0x11A:	// both signals high, a greater than b
		// check to see if the signals have crossed
		if (b > a + SIGNAL_CROSSING_MARGIN)
		{
			aThreshold = a - HYSTERESIS;
			bThreshold = b - HYSTERESIS;
			++phaseAccumulator;
			phase = 0x11B;
			break;
		}

		// check to see if b has changed from high to low
		if (b < bThreshold)
		{
			--phaseAccumulator;
			phase = 0x10A;
		}
		break;

	case 0x11B:	// both signals high, b greater than a
		// check to see if the signals have crossed
		if (a > b + SIGNAL_CROSSING_MARGIN)
		{
			aThreshold = a - HYSTERESIS;
			bThreshold = b - HYSTERESIS;
			--phaseAccumulator;
			phase = 0x11A;
			break;
		}

		// check to see if a has changed from high to low
		if (a < aThreshold)
		{
			++phaseAccumulator;
			phase = 0x01B;
		}
		break;
	}

	// Update our output... we change our output on every half cycle, thus
	// every time we accumulate three phase changes in the same direction.
	// This is our debouncing, too.
	if (phaseAccumulator >= 3)
	{
		phaseAccumulator -= 3;
		++currentValue;
	}
	else if (phaseAccumulator <= -3)
	{
		phaseAccumulator += 3;
		--currentValue;
	}
}


