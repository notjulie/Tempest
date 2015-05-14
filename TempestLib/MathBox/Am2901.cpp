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

#include "ALULogEntry.h"
#include "MathBoxException.h"
#include "MathBoxTracer.h"

#include "Am2901.h"

Am2901::Am2901(void)
{
	clock = false;
	CarryIn = false;
	I012 = 0;
	I345 = 0;
	I678 = 0;
}


Tristate Am2901::GetC3(const NullableNybble &R, const NullableNybble &S) const
{
	NullableNybble P = R | S;
	NullableNybble G = R & S;
	return
		G[2] |
		(P[2] & G[1]) |
		(P[2] & P[1] & G[0]) |
		(P[2] & P[1] & P[0] & CarryIn);
}

Tristate Am2901::GetC4(const NullableNybble &R, const NullableNybble &S) const
{
	NullableNybble P = R | S;
	NullableNybble G = R & S;
	return
		G[3] |
		(P[3] & G[2]) |
		(P[3] & P[2] & G[1]) |
		(P[3] & P[2] & P[1] & G[0]) |
		(P[3] & P[2] & P[1] & P[0] & CarryIn);
}

Tristate Am2901::GetXORCarry(const NullableNybble &R, const NullableNybble &S)
{
	NullableNybble P = R | S;
	NullableNybble G = R & S;

	Tristate notResult =
		G[3] |
		(P[3] & G[2]) |
		(P[3] & P[2] & G[1]) |
		(P[3] & P[2] & P[1] & P[0] & (G[0] & !CarryIn));
	return !notResult;

}

Tristate Am2901::GetXOROverflow(const NullableNybble &R, const NullableNybble &S) const
{
	NullableNybble P = R | S;
	NullableNybble G = R & S;

	Tristate a =
		P[2] |
		(G[2] & P[1]) |
		(G[2] & G[1] & P[0]) |
		(G[2] & G[1] & G[0] & CarryIn);
	Tristate b =
		P[3] |
		(G[3] & P[2]) |
		(G[3] & G[2] & P[1]) |
		(G[3] & G[2] & G[1] & P[0]) |
		(G[3] & G[2] & G[1] & G[0] & CarryIn);
	return a ^ b;
}

Tristate Am2901::GetCarryOut(void)
{
	// Get R and S
	NullableNybble R = GetR();
	NullableNybble S = GetS();

	switch (I345)
	{
	case 0:
		return GetC4(R, S);

	case 1:
		return GetC4(~R, S);

	case 2:
		return GetC4(R, ~S);

	case 3:
		return ((R | S) != Nybble(0xF)) || CarryIn;

	case 4:
		return ((R & S) != Nybble(0x0)) || CarryIn;

	case 5:
		return (((~R) & S) != Nybble(0x0)) || CarryIn;

	case 6:
		return GetXORCarry(~R, S);

	case 7:
		return GetXORCarry(R, S);

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetCarryOut not implemented for function: %d", I345);
			throw MathBoxException(buf);
		}
	}
}

bool Am2901::GetF3(void) const
{
	return (GetF().Value() & 8) != 0;
}

Tristate Am2901::GetOVR(void) const
{
	// Get R and S
	NullableNybble R = GetR();
	NullableNybble S = GetS();

	switch (I345)
	{
	case 0:
		return GetC3(R, S) ^ GetC4(R, S);

	case 1:
		return GetC3(~R, S) ^ GetC4(~R, S);

	case 2:
		return GetC3(R, ~S) ^ GetC4(R, ~S);

	case 3:
		return ((R | S) != Nybble(0xF)) || CarryIn;

	case 4:
		return ((R & S) != Nybble(0x0)) || CarryIn;

	case 5:
		return (((~R) & S) != Nybble(0x0)) || CarryIn;

	case 6:
		return GetXOROverflow(~R, S);

	case 7:
		return GetXOROverflow(R, S);

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetOVR not implemented for function: %d", I345);
			throw MathBoxException(buf);
		}
	}
}

Tristate Am2901::GetQ0Out(void)
{
	switch (I678)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 6:
	case 7:
		return Tristate::Unknown;

	case 4:
	case 5:
		return (QLatch.Value() & 1) != 0;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetQ0Out not implemented for destination: %d", I678);
			throw MathBoxException(buf);
		}
	}
}

Tristate Am2901::GetQ3Out(void)
{
	switch (I678)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		return Tristate::Unknown;

	case 6:
	case 7:
		return (QLatch.Value() & 8) != 0;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetQ3 not implemented for destination: %d", I678);
			throw MathBoxException(buf);
		}
	}
}

Nybble Am2901::GetA(void) const
{
	// if the clock is high we return the current value; else we
	// return the latched value
	if (clock)
		return RAM[AAddress.Value()];
	else
		return ALatch;
}

Nybble Am2901::GetB(void) const
{
	// if the clock is high we return the current value; else we
	// return the latched value
	if (clock)
		return RAM[BAddress.Value()];
	else
		return BLatch;
}

Nybble Am2901::GetF(void) const
{
	switch (I345)
	{
	case 0:
		if (CarryIn)
			return GetR() + GetS() + Nybble(1);
		else
			return GetR() + GetS();

	case 1:
		if (CarryIn)
			return GetS() - GetR();
		else
			return GetS() - GetR() - Nybble(1);

	case 2:
		if (CarryIn)
			return GetR() - GetS();
		else
			return GetR() - GetS() - Nybble(1);

	case 3:
		return GetR() | GetS();

	case 4:
		return GetR() & GetS();

	case 5:
		return (~GetR()) & GetS();

	case 6:
		return GetR() ^ GetS();

	case 7:
		return ~(GetR() ^ GetS());

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetF not implemented for function: %d", I345);
			throw MathBoxException(buf);
		}
	}
}

Nybble Am2901::GetY(void)
{
	switch (I678)
	{
	case 0:
	case 1:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		return GetF();

	case 2:
		return GetA();

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetY not implemented for destination: %d", I678);
			throw MathBoxException(buf);
		}
	}
}

Nybble Am2901::GetR(void) const
{
	switch (I012)
	{
	case 0:
	case 1:
		return GetA();

	case 2:
	case 3:
	case 4:
		return 0;

	case 5:
	case 6:
	case 7:
		return DataIn;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetR not implemented for source: %d", I012);
			throw MathBoxException(buf);
		}
	}
}


Nybble Am2901::GetS(void) const
{
	switch (I012)
	{
	case 0:
	case 2:
	case 6:
		return QLatch;

	case 1:
	case 3:
		return GetB();

	case 4:
	case 5:
		return GetA();

	case 7:
		return 0;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetS not implemented for source: %d", I012);
			throw MathBoxException(buf);
		}
	}
}

Tristate Am2901::GetRAM0Out(void)
{
	switch (I678)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 6:
	case 7:
		return Tristate::Unknown;

	case 4:
	case 5:
		return (GetF().Value() & 1) != 0;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:GetRAM0Out not implemented for destination: %d", I678);
			throw MathBoxException(buf);
		}
	}
}


Tristate Am2901::GetRAM3Out(void)
{
	switch (I678)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		return Tristate::Unknown;

	case 6:
	case 7:
		return (GetF().Value() & 8) != 0;

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
	// handle rising edges
	if (!clock && newClockState)
	{
		// latch values to the memory pointed to by the BAddress and to the Q register
		// according to the destination code
		// to RAM...
		switch (I678)
		{
		case 0:
		case 1:
			break;

		case 2:
		case 3:
			RAM[BAddress.Value()] = GetF();
			break;

		case 4:
		case 5:
		{
			Nybble shifted = GetF() >> 1;
			if (RAM3In.Value())
				shifted = shifted | Nybble(8);
			RAM[BAddress.Value()] = shifted;
			break;
		}

		case 6:
		case 7:
		{
			Nybble shifted = GetF() << 1;
			if (RAM0In.Value())
				shifted = shifted | Nybble(1);
			RAM[BAddress.Value()] = shifted;
			break;
		}

		default:
		{
			char buf[200];
			sprintf_s(buf, "Am2901:SetClock RAM latch not implemented for destination: %d", I678);
			throw MathBoxException(buf);
		}
		}

		// to Q...
		switch (I678)
		{
		case 0:
			QLatch = GetF();
			break;

		case 1:
		case 2:
		case 3:
		case 5:
		case 7:
			break;

		case 4:
			QLatch = QLatch >> 1;
			if (Q3In.Value())
				QLatch = QLatch | Nybble(8);
			break;

		case 6:
			QLatch = QLatch << 1;
			if (Q0In.Value())
				QLatch = QLatch | Nybble(1);
			break;

		default:
			{
				char buf[200];
				sprintf_s(buf, "Am2901:SetClock Q latch not implemented for destination: %d", I678);
				throw MathBoxException(buf);
			}
		}
	}

	// handle falling edges
	if (clock && !newClockState)
	{
		// latch A and B
		ALatch = RAM[AAddress.Value()];
		BLatch = RAM[BAddress.Value()];
	}

	clock = newClockState;
}


ALULogEntry Am2901::GetLogData(void) const
{
	ALULogEntry result;

	result.A = AAddress;
	result.B = BAddress;
	result.Cn = CarryIn;
	result.DataIn = DataIn;
	result.F3 = GetF3();
	result.I012 = I012;
	result.I345 = I345;
	result.I678 = I678;
	result.OVR = GetOVR();
	result.QLatch = QLatch;
	result.R = GetR();
	result.S = GetS();

	for (int i = 0; i < 16; ++i)
		result.RAM[i] = RAM[i];

	return result;
}

