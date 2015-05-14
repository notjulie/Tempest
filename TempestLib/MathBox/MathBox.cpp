// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Emulation of the MathBox; this emulates all the logic external
//    to the 4 Am2901 chips... those are emulated in class Am2901.
// ====================================================================


#include "stdafx.h"

#include "MathBoxException.h"

#include "MathBox.h"


MathBox::MathBox(void)
{
}

MathBox::~MathBox(void)
{
}

void MathBox::LoadROM(const uint8_t *rom, int length, char slot)
{
	switch (slot)
	{
	case 'A':
		romA.resize((unsigned)length);
		memcpy(&romA[0], rom, (unsigned)length);
		break;

	case 'E':
		romE.resize((unsigned)length);
		memcpy(&romE[0], rom, (unsigned)length);
		break;

	case 'F':
		romF.resize((unsigned)length);
		memcpy(&romF[0], rom, (unsigned)length);
		break;

	case 'H':
		romH.resize((unsigned)length);
		memcpy(&romH[0], rom, (unsigned)length);
		break;

	case 'J':
		romJ.resize((unsigned)length);
		memcpy(&romJ[0], rom, (unsigned)length);
		break;

	case 'K':
		romK.resize((unsigned)length);
		memcpy(&romK[0], rom, (unsigned)length);
		break;

	case 'L':
		romL.resize((unsigned)length);
		memcpy(&romL[0], rom, (unsigned)length);
		break;

	default:
		throw MathBoxException("MathBox::LoadROM: invalid slot letter");
	}
}


uint8_t MathBox::GetStatus(void)
{
	// it appears that we are supposed to return 0x80 to indicate that
	// the mathbox is busy; this is never true because the emulated mathbox
	// does all its work synchronously
	return 0;
}

uint8_t MathBox::ReadLow(void)
{
	NullableNybble yK = aluK.GetY();
	NullableNybble yF = aluF.GetY();
	if (yK.IsUnknown() || yF.IsUnknown())
		throw MathBoxException("MathBox::ReadLow: value unknown");

	return (uint8_t)((yF.Value().Value() << 4) +yK.Value().Value());
}

uint8_t MathBox::ReadHigh(void)
{
	NullableNybble yJ = aluJ.GetY();
	NullableNybble yE = aluE.GetY();
	if (yJ.IsUnknown() || yE.IsUnknown())
		throw MathBoxException("MathBox::ReadHigh: value unknown");

	return (uint8_t)((yE.Value().Value() << 4) + yJ.Value().Value());
}

void MathBox::Write(uint8_t address, uint8_t value)
{
	try
	{
		// set our inputs... the address strobe will assert BEGIN and cause the
		// first rising edge of the clock
		addressIn = address;
		dataIn = value;
		BEGIN = true;
		HandleRisingClock();

		// shortly after the clock will fall
		HandleFallingClock();

		// BEGIN clears, but since our clock runs at twice the speed of the main
		// CPU clock, the data and address lines will still be active for the beginning
		// of the next cycle
		BEGIN = false;
		if (STOP.Value())
			return;

		// do the second cycle
		HandleRisingClock();
		HandleFallingClock();

		// now our inputs will clear
		addressIn = NullableByte::Unknown;
		dataIn = NullableByte::Unknown;

		// then we can just handle clock pulses until the clock is disabled
		while (!STOP.Value())
		{
			HandleRisingClock();
			HandleFallingClock();
		}
	}
	catch (MathBoxException &x)
	{
		SetError(x.what());
	}
	catch (...)
	{
		SetError("Unknown exception in MathBox::Write");
	}
}


void MathBox::HandleRisingClock(void)
{
	// calculate the new PC 
	NullableByte newPC;
	Tristate pcen = GetTristate(PCEN);
	if (pcen.IsUnknown())
		throw MathBoxException("MathBox::HandleRisingClock: PCEN is unknown");

	if (pcen.Value())
	{
		// we load the PC from whichever source is selected
		if (BEGIN.Value())
		{
			if (addressIn.IsUnknown())
				throw MathBoxException("Load PC from ROM A: addressIn not set");
			newPC = romA[(unsigned)addressIn.Value()];
		}
		else
		{
			newPC = JumpLatch;
		}
	}
	else
	{
		if (PC.IsUnknown())
			throw MathBoxException("Can't increment PC... not set");
		newPC = (uint8_t)(PC.Value() + 1);
		if (newPC.Value() == 0)
			throw MathBoxException("PC wraparound");
	}

	// calculate the new value of Q0Latch
	Tristate newQ0Latch = GetTristate(Q0);

	// let the ALUs handle the rising clock edge...
	SetALUInputs();
	aluK.SetClock(true);
	aluF.SetClock(true);
	aluJ.SetClock(true);
	aluE.SetClock(true);

	// latch all the state values that we are supposed to latch on the
	// rising clock
	PC = newPC;
	Q0Latch = newQ0Latch;
}

void MathBox::HandleFallingClock(void)
{
	// calculate the new value of STOP
	Tristate newSTOP;
	if (BEGIN.Value())
		newSTOP = false;
	else
		newSTOP = GetTristate(A12);

	// new value of our jump address latch
	NullableByte newJumpLatch = JumpLatch;
	Tristate ldab = GetTristate(LDAB);
	if (ldab.IsUnknown() || PC.IsUnknown())
		newJumpLatch = NullableByte::Unknown;
	else if (ldab.Value())
		newJumpLatch = (uint8_t)((romL[PC.Value()]<<4) + romK[PC.Value()]);

	// let the ALUs handle the falling clock edge...
	SetALUInputs();

	// When the ALU clock is high is when its gates are active... now is the time to
	// set everybody's carry flags...
	SetALUCarryFlags();

	// dropping the clock latches the result
	aluK.SetClock(false);
	aluF.SetClock(false);
	aluJ.SetClock(false);
	aluE.SetClock(false);

	// latch all the state values that we are supposed to latch on the
	// falling clock
	STOP = newSTOP;
	JumpLatch = newJumpLatch;
}

Tristate MathBox::GetTristate(Bit bit)
{
	char buf[200];

	switch (bit)
	{
	case A10:
		if (PC.IsUnknown())
			return Tristate::Unknown;
		return ((romJ[PC.Value()] & 2) != 0);

	case A10STAR:
		return GetTristate(A10) ^ (GetTristate(M) && Q0Latch);

	case A12:
		if (PC.IsUnknown())
			return Tristate::Unknown;
		return ((romH[PC.Value()] & 8) != 0);

	case A18:
		if (PC.IsUnknown())
			return Tristate::Unknown;
		return ((romF[PC.Value()] & 2) != 0);

	case C:
		if (PC.IsUnknown())
			return Tristate::Unknown;
		return ((romE[PC.Value()] & 1) != 0);

	case J:
		if (PC.IsUnknown())
			return Tristate::Unknown;
		return ((romE[PC.Value()] & 4) != 0);

	case LDAB:
		if (PC.IsUnknown())
			return Tristate::Unknown;
		return ((romF[PC.Value()] & 8) != 0);

	case M:
		if (PC.IsUnknown())
			return Tristate::Unknown;
		return ((romE[PC.Value()] & 2) != 0);

	case PCEN:
	{
		Tristate E5XORout = GetTristate(S0) ^ GetTristate(S1);
		Tristate D4NAND1out = !(E5XORout && GetTristate(S));
		Tristate D4NAND2out = !(D4NAND1out && GetTristate(J));
		return BEGIN || !D4NAND2out;
	}

	case Q0:
		{
			Tristate aluKQ0 = aluK.GetQ0Out();
			Tristate a18 = GetTristate(A18);

			if (!aluKQ0.IsUnknown())
			{
				// ASSUMPTION WARNING
				// we have multiple sources driving Q0... I will assume that they are
				// wired OR and see how that goes
				return aluKQ0 || !a18;
			}

			return !a18;
		}

	case R15:
	{
		Tristate E5XORout = GetTristate(S0) ^ GetTristate(S1);
		Tristate D4NAND1out = !(E5XORout && GetTristate(S));
		return !(D4NAND1out && !GetTristate(A18));
	}

	case S:
		if (PC.IsUnknown())
			return Tristate::Unknown;
		return ((romE[PC.Value()] & 8) != 0);

	case S0:
		return aluE.GetOVR();

	case S1:
		return aluE.GetF3();

	default:
		sprintf_s(buf, "MathBox::GetTristate: unsupported bit: %d", bit);
		throw MathBoxException(buf);
	}
}


void MathBox::SetALUInputs(void)
{
	// If the PC is not set that should mean that this is our first rising edge;
	// assuming that be so we just set all the ALU inputs to their unknown state.
	// This is just a diagnostic measure so that if the ALU gets asked to do something
	// that doesn't make sense in this state it can throw an exception.
	if (PC.IsUnknown())
	{
		aluK.AAddress = aluF.AAddress = aluJ.AAddress = aluE.AAddress = Nybble();
		aluK.BAddress = aluF.BAddress = aluJ.BAddress = aluE.BAddress = Nybble();
		aluK.I012 = aluF.I012 = aluJ.I012 = aluE.I012 = 0;
		aluK.I345 = aluF.I345 = aluJ.I345 = aluE.I345 = 0;
		aluK.I678 = aluF.I678 = aluJ.I678 = aluE.I678 = 0;
		return;
	}

	// the A & B inputs to the ALUs are all the same
	aluK.AAddress = aluF.AAddress = aluJ.AAddress = aluE.AAddress = romL[PC.Value()];
	aluK.BAddress = aluF.BAddress = aluJ.BAddress = aluE.BAddress = romK[PC.Value()];

	// so are these
	aluK.I345 = aluF.I345 = aluJ.I345 = aluE.I345 = (uint8_t)(romH[PC.Value()] & 7);
	aluK.I678 = aluF.I678 = aluJ.I678 = aluE.I678 = (uint8_t)(romF[PC.Value()] & 7);

	// I012 are a little more complicated
	int i01 = romJ[PC.Value()] & 1;
	if (GetTristate(A10STAR).Value())
		i01 += 2;
	aluK.I012 = aluF.I012 = (uint8_t)(i01 + (romJ[PC.Value()] & 4));
	aluJ.I012 = aluE.I012 = (uint8_t)(i01 + ((romJ[PC.Value()] & 8) >> 1));

	// set the data inputs accordingly
	if (dataIn.IsUnknown())
	{
		aluK.DataIn = aluF.DataIn = aluJ.DataIn = aluE.DataIn = Nybble();
	}
	else
	{
		aluK.DataIn = aluJ.DataIn = (uint8_t)(dataIn.Value() & 0xF);
		aluF.DataIn = aluE.DataIn = (uint8_t)(dataIn.Value() >> 4);
	}
}


void MathBox::SetALUCarryFlags(void)
{
	// the actual carry flags are easy... they just cascade up
	aluK.CarryIn = GetTristate(C).Value();
	aluF.CarryIn = aluK.GetCarryOut().Value();
	aluJ.CarryIn = aluF.GetCarryOut().Value();
	aluE.CarryIn = aluJ.GetCarryOut().Value();

	// Pass the outputs of ALUs to the inputs of other ALUs... potentially this
	// could require multiple iterations, for example carry flags on one ALU
	// could cause a change in its carry out.  So we iterate a few times, well
	// more than we need just in case.
	for (int i = 0; i < 2; ++i)
	{
		// RAM0, RAM3, Q0 and Q3 are all tristate... which is output and which is input depends
		// on the current ALU operation; thus you'll see a lot of cases where they are both being
		// sent both directions.  The called function will sort things out depending on who is
		// reporting an unknown value.
		aluK.SetQ0In(GetTristate(Q0));
		aluK.SetQ3In(aluF.GetQ0Out());
		aluK.SetRAM0In(aluE.GetQ3Out());
		aluK.SetRAM3In(aluF.GetRAM0Out());

		aluF.SetQ0In(aluK.GetQ3Out());
		aluF.SetQ3In(aluJ.GetQ0Out());
		aluF.SetRAM0In(aluK.GetRAM3Out());
		aluF.SetRAM3In(aluJ.GetRAM0Out());

		aluJ.SetQ0In(aluF.GetQ3Out());
		aluJ.SetQ3In(aluE.GetQ0Out());
		aluJ.SetRAM0In(aluF.GetRAM3Out());
		aluJ.SetRAM3In(aluE.GetRAM0Out());

		aluE.SetQ0In(aluJ.GetQ3Out());
		aluE.SetQ3In(aluK.GetRAM0Out());
		aluE.SetRAM0In(aluJ.GetRAM3Out());
		aluE.SetRAM3In(GetTristate(R15));
	}
}

void MathBox::SetError(const std::string &_status)
{
	// we only record the first error
	if (error.size() == 0)
		error = _status;
}



