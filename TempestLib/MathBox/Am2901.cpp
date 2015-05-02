// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Emulation of the Am2901 ALU chip
// ====================================================================

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
	switch (I345)
	{
	case 3:
		{
			uint8_t p = (uint8_t)(GetR() | GetS());
			return (p != 0xF) || CarryIn;
		}

	case -1:
	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetCarryOut not implemented for function: %d", I345);
			throw MathBoxException(buf);
		}
	}
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
		return Tristate::Unknown;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetQ3 not implemented for destination: %d", I678);
			throw MathBoxException(buf);
		}
	}
}

NullableByte Am2901::GetB(void)
{
	throw MathBoxException("Am2901::GetB not implemented");
}

uint8_t Am2901::GetR(void)
{
	switch (I012)
	{
	case 3:
		return 0;

	case 7:
		if (DataIn < 0)
			throw MathBoxException("MathBox::GetR: DataIn not set");
		return (uint8_t)DataIn;

	case -1:
	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetR not implemented for source: %d", I012);
			throw MathBoxException(buf);
		}
	}
}


NullableByte Am2901::GetS(void)
{
	switch (I012)
	{
	case 3:
		return GetB();

	case 7:
		return 0;

	case -1:
	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetS not implemented for source: %d", I012);
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
		return Tristate::Unknown;

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
