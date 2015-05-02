
#include "stdafx.h"

#include "MathBoxException.h"

#include "Am2901.h"

Am2901::Am2901(void)
{
	// start out with our inputs marked as unknown
	clockIsSet = false;
	AAddress = -1;
	BAddress = -1;
}


bool Am2901::GetCarryOut(void)
{
	throw MathBoxException("Am2901::GetCarryOut not implemented");
}

Tristate Am2901::GetQ3(void)
{
	switch (I678)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 5:
	case -1:
		return TS_UNKNOWN;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetQ3 not implemented for destination: %d", I678);
			throw MathBoxException(buf);
		}
	}
}

Tristate Am2901::GetRAM3(void)
{
	switch (I678)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case -1:
		return TS_UNKNOWN;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetRAM3 not implemented for destination: %d", I678);
			throw MathBoxException(buf);
		}
	}
}


void Am2901::SetClock(bool newClockState)
{
	// if the clock is not set just set it
	if (!clockIsSet)
	{
		clockIsSet = true;
		clockState = newClockState;
		return;
	}

	// handle rising edges
	if (!clockState && newClockState)
	{
		throw MathBoxException("Am2901::SetClock doesn't handle rising edges");
	}

	// handle falling edges
	if (clockState && !newClockState)
	{
		throw MathBoxException("Am2901::SetClock doesn't handle falling edges");
	}

	clockState = newClockState;
}
