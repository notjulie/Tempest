
#include "Encoder.h"

// Our threshold between high and low is midway between where the
// signals cross on the low side and where they cross on the high side.
// We need a nominal threshold to distinguish what we consider low and
// high, and then average the crossings on either side.
static const uint16_t NOMINAL_THRESHOLD = 20000;

Encoder::Encoder(void)
{
	// clear
	isInitialized = false;
	wasInput1Higher = false;
	wasHigh = false;
	currentValue = 0;

	lowCrossingLevel = NOMINAL_THRESHOLD;
	highCrossingLevel = NOMINAL_THRESHOLD;
}


void Encoder::AddSample(int input1, int input2)
{
	// interpret the inputs
	int inputSum = input1 + input2;
	int threshold = (lowCrossingLevel + highCrossingLevel) / 2;
	bool isHigh = inputSum > threshold;
	bool isInput1Higher = (input1 > input2);

	// if this is the first sample just set our state according to
	// the inputs and exit
	if (!isInitialized)
	{
		wasInput1Higher = isInput1Higher;
		wasHigh = isHigh;
		isHigh = (inputSum > threshold);
		isInitialized = true;
		return;
	}

	// if the signals didn't cross we don't need to do anything
	if (isInput1Higher == wasInput1Higher)
		return;

	// We determine our actual threshold as the midpoint between the
	// average of high crossings and low crossings; in order to do this
	// objectively we determine which side we're on based on a nominal
	// constant.  If we based the threshold calculation on the calculated
	// threshold we could end up with a positive feedback loop and the
	// threshold could be completely mangled.
	if (inputSum > NOMINAL_THRESHOLD)
	{
		highCrossingLevel = (99*highCrossingLevel + inputSum + 50) / 100;
	}
	else
	{
		lowCrossingLevel = (99*lowCrossingLevel + inputSum + 50) / 100;
	}

	// if our high/low state is different from the previous crossing
	// that means we moved
	if (isHigh && !wasHigh)
	{
		if (wasInput1Higher)
			++currentValue;
		else
			--currentValue;
	}
	else if (wasHigh && !isHigh)
	{
		if (wasInput1Higher)
			--currentValue;
		else
			++currentValue;
	}

	// record the state
	wasHigh = isHigh;
	wasInput1Higher = isInput1Higher;
}
