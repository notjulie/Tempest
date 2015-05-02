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
}


Tristate Am2901::GetCarryOut(void)
{
	if (I345.IsUnknown())
		return Tristate::Unknown;

	switch (I345.Value())
	{
	case 3:
		{
			NullableByte p = GetR() | GetS();
			return (p != 0xF) || CarryIn;
		}

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetCarryOut not implemented for function: %d", I345.Value());
			throw MathBoxException(buf);
		}
	}
}

Tristate Am2901::GetF3(void)
{
	NullableByte f = GetF();
	if (f.IsUnknown())
		return Tristate::Unknown;
	else
		return (f.Value() & 0x8) != 0;
}

Tristate Am2901::GetOVR(void)
{
	if (I345.IsUnknown())
		return Tristate::Unknown;

	switch (I345.Value())
	{
	case 3:
		{
			NullableByte p = GetR() | GetS();
			return (p != 0xF) || CarryIn;
		}

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetOVR not implemented for function: %d", I345.Value());
			throw MathBoxException(buf);
		}
	}
}


Tristate Am2901::GetQ3(void)
{
	if (I678.IsUnknown())
		return Tristate::Unknown;

	switch (I678.Value())
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
			sprintf_s(buf, "Am2901:GetQ3 not implemented for destination: %d", I678.Value());
			throw MathBoxException(buf);
		}
	}
}

NullableByte Am2901::GetB(void)
{
	// if the clock is high we return the current value; else we
	// return the latched value
	if (clock.IsUnknown())
		return NullableByte::Unknown;
	else if (clock.Value())
		return GetRAMValue(BAddress);
	else
		return BLatch;
}

NullableByte Am2901::GetF(void)
{
	if (I345.IsUnknown())
		return NullableByte::Unknown;

	switch (I345.Value())
	{
	case 3:
		return GetR() | GetS();

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetF not implemented for function: %d", I345.Value());
			throw MathBoxException(buf);
		}
	}
}

NullableByte Am2901::GetR(void)
{
	if (I012.IsUnknown())
		return NullableByte::Unknown;

	switch (I012.Value())
	{
	case 3:
		return 0;

	case 7:
		return DataIn;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetR not implemented for source: %d", I012.Value());
			throw MathBoxException(buf);
		}
	}
}


NullableByte Am2901::GetS(void)
{
	if (I012.IsUnknown())
		return NullableByte::Unknown;

	switch (I012.Value())
	{
	case 3:
		return GetB();

	case 7:
		return 0;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetS not implemented for source: %d", I012.Value());
			throw MathBoxException(buf);
		}
	}
}

NullableByte Am2901::GetRAMValue(const NullableByte &_address)
{
	if (_address.IsUnknown())
		return NullableByte::Unknown;
	else
		return RAM[_address.Value()];
}

Tristate Am2901::GetRAM3(void)
{
	if (I678.IsUnknown())
		return Tristate::Unknown;

	switch (I678.Value())
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
			sprintf_s(buf, "Am2901:GetRAM3 not implemented for destination: %d", I678.Value());
			throw MathBoxException(buf);
		}
	}
}


void Am2901::SetClock(bool newClockState)
{
	// if the clock is not set just set it
	if (clock.IsUnknown())
	{
		clock = newClockState;
		return;
	}

	// handle rising edges
	if (!clock.Value() && newClockState)
	{
		// latch values to the memory pointed to by the BAddress and to the Q register
		// according to the destination code
		if (I678.IsUnknown())
			throw MathBoxException("Am2901::SetClock: destination code unknown");

		// to RAM...
		switch (I678.Value())
		{
		case 2:
		case 3:
			WriteToRAM(BAddress, GetF());
			break;

		default:
			{
				char buf[200];
				sprintf_s(buf, "Am2901:SetClock RAM latch not implemented for destination: %d", I678.Value());
				throw MathBoxException(buf);
			}
		}

		// to Q...
		switch (I678.Value())
		{
		case 1:
		case 2:
		case 3:
		case 5:
		case 7:
			break;

		default:
			{
				char buf[200];
				sprintf_s(buf, "Am2901:SetClock Q latch not implemented for destination: %d", I678.Value());
				throw MathBoxException(buf);
			}
		}
	}

	// handle falling edges
	if (clock.Value() && !newClockState)
	{
		// latch A and B
		ALatch = GetRAMValue(AAddress);
		BLatch = GetRAMValue(BAddress);
	}

	clock = newClockState;
}


void Am2901::WriteToRAM(const NullableByte &_address, const NullableByte &_value)
{
	if (_address.IsUnknown())
		throw MathBoxException("Am2901::WriteToRAM: address not specified");
	RAM[_address.Value()] = _value;
}
