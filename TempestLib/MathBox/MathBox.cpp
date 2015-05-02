
#include "stdafx.h"

#include "MathBoxException.h"

#include "MathBox.h"


MathBox::MathBox(void)
{
	Q0Latch = TS_UNKNOWN;
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
	SetError("MathBox::GetStatus not implemented");
	return 0;
}

uint8_t MathBox::Read1(void)
{
	SetError("MathBox::Read1 not implemented");
	return 0;
}

uint8_t MathBox::Read2(void)
{
	SetError("MathBox::Read2 not implemented");
	return 0;
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

		// our inputs will clear
		addressIn = -1;
		dataIn = -1;
		BEGIN = false;

		// then we can just handle clock pulses until the clock is disabled
		while (!STOP)
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
	int newPC;
	if (GetBit(PCEN))
	{
		// we load the PC from whichever source is selected
		if (BEGIN)
		{
			if (addressIn < 0)
				throw MathBoxException("Load PC from ROM A: addressIn not set");
			newPC = romA[(unsigned)addressIn];
		}
		else
		{
			throw MathBoxException("Load PC from latch B1");
		}
	}
	else
	{
		if (PC < 0)
			throw MathBoxException("Can't increment PC... not set");
		newPC = PC + 1;
		if (newPC > 255)
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
	bool newSTOP;
	if (BEGIN)
		newSTOP = false;
	else
		newSTOP = GetBit(A12);

	// let the ALUs handle the falling clock edge...
	SetALUInputs();

	// When the ALU clock is high is when its gates are active... now is the time to
	// set everybody's carry flags... do it iteratively a couple times just to make sure...
	// there is some feedback here
	SetALUCarryFlags();
	SetALUCarryFlags();
	SetALUCarryFlags();
	SetALUCarryFlags();
	SetALUCarryFlags();

	// dropping the clock latches the result
	aluK.SetClock(false);
	aluF.SetClock(false);
	aluJ.SetClock(false);
	aluE.SetClock(false);

	// latch all the state values that we are supposed to latch on the
	// falling clock
	STOP = newSTOP;
}

Tristate MathBox::GetTristate(Bit bit)
{
	char buf[200];

	switch (bit)
	{
	case A10:
		if (PC < 0)
			return TS_UNKNOWN;
		return ((romJ[(unsigned)PC] & 2) != 0) ? TS_ON : TS_OFF;

	case A18:
		if (PC < 0)
			return TS_UNKNOWN;
		return ((romF[(unsigned)PC] & 2) != 0) ? TS_ON : TS_OFF;

	case C:
		if (PC < 0)
			return TS_UNKNOWN;
		return ((romE[(unsigned)PC] & 1) != 0) ? TS_ON : TS_OFF;

	case M:
		if (PC < 0)
			return TS_UNKNOWN;
		return ((romE[(unsigned)PC] & 2) != 0) ? TS_ON : TS_OFF;

	case Q0:
		switch (GetTristate(A18))
		{
		case TS_ON: return TS_OFF;
		case TS_OFF: return TS_ON;

		case TS_UNKNOWN:
		default:
			return TS_UNKNOWN;
		}

	case A10STAR:
	case A12:
	case J:
	case PCEN:
	case S:
	case S0:
	case S1:
	default:
		sprintf_s(buf, "MathBox::GetTristate: unsupported bit: %d", bit);
		throw MathBoxException(buf);
	}
}


bool MathBox::GetBit(Bit bit)
{
	char buf[200];

	switch (bit)
	{
	case A10STAR:
	{
		bool E4ANDout = false;
		if (GetBit(M))
		{
			if (Q0Latch == TS_UNKNOWN)
				throw MathBoxException("A10STAR: Q0Latch is unknown");
			E4ANDout = Q0Latch == TS_ON;
		}
		return E4ANDout ^ GetBit(A10);
	}

	case PCEN:
	{
		// if BEGIN is set we don't care about the rest, which may be
		// in unspecified states
		if (BEGIN)
			return true;
		bool E5XORout = GetBit(S0) ^ GetBit(S1);
		bool D4NAND1out = !(E5XORout & GetBit(S));
		bool D4NAND2out = !(D4NAND1out & GetBit(J));
		return BEGIN | !D4NAND2out;
	}

	case A10:
	case A12:
	case A18:
	case C:
	case J:
	case M:
	case Q0:
	case S:
	case S0:
	case S1:
	default:
		switch (GetTristate(bit))
		{
		case TS_ON: return true;
		case TS_OFF: return false;

		case TS_UNKNOWN:
		default:
			{
				sprintf_s(buf, "MathBox::GetBit: bit value unknown: %d", bit);
				throw MathBoxException(buf);
			}
		}
	}
}

void MathBox::SetALUInputs(void)
{
	// If the PC is not set that should mean that this is our first rising edge;
	// assuming that be so we just set all the ALU inputs to their unknown state.
	// This is just a diagnostic measure so that if the ALU gets asked to do something
	// that doesn't make sense in this state it can throw an exception.
	if (PC < 0)
	{
		aluK.AAddress = aluF.AAddress = aluJ.AAddress = aluE.AAddress = -1;
		aluK.BAddress = aluF.BAddress = aluJ.BAddress = aluE.BAddress = -1;
		aluK.I012 = aluF.I012 = aluJ.I012 = aluE.I012 = -1;
		aluK.I345 = aluF.I345 = aluJ.I345 = aluE.I345 = -1;
		aluK.I678 = aluF.I678 = aluJ.I678 = aluE.I678 = -1;
		return;
	}

	// the A & B inputs to the ALUs are all the same
	aluK.AAddress = aluF.AAddress = aluJ.AAddress = aluE.AAddress = romL[(unsigned)PC];
	aluK.BAddress = aluF.BAddress = aluJ.BAddress = aluE.BAddress = romK[(unsigned)PC];

	// so are these
	aluK.I345 = aluF.I345 = aluJ.I345 = aluE.I345 = romH[(unsigned)PC] & 7;
	aluK.I678 = aluF.I678 = aluJ.I678 = aluE.I678 = romF[(unsigned)PC] & 7;

	// I012 are a little more complicated
	int i01 = romJ[(unsigned)PC] & 1;
	if (GetBit(A10STAR))
		i01 += 2;
	aluK.I012 = aluF.I012 = i01 + (romJ[(unsigned)PC] & 4);
	aluJ.I012 = aluE.I012 = i01 + ((romJ[(unsigned)PC] & 8) >> 1);

	// set the data inputs accordingly
	if (dataIn < 0)
	{
		aluK.DataIn = aluF.DataIn = aluJ.DataIn = aluE.DataIn = -1;
	}
	else
	{
		aluK.DataIn = aluJ.DataIn = dataIn & 0xF;
		aluF.DataIn = aluE.DataIn = dataIn >> 4;
	}
}

void MathBox::SetALUCarryFlags(void)
{
	aluK.RAM0 = aluE.GetQ3();
	aluK.Q0 = GetTristate(Q0);
	aluK.CarryIn = GetBit(C);

	aluF.RAM0 = aluK.GetRAM3();
	aluF.Q0 = aluK.GetQ3();
	aluF.CarryIn = aluK.GetCarryOut();

	aluJ.RAM0 = aluF.GetRAM3();
	aluJ.Q0 = aluF.GetQ3();
	aluJ.CarryIn = aluF.GetCarryOut();

	aluE.RAM0 = aluJ.GetRAM3();
	aluE.Q0 = aluJ.GetQ3();
	aluE.CarryIn = aluJ.GetCarryOut();
}

void MathBox::SetError(const std::string &_status)
{
	// we only record the first error
	if (error.size() == 0)
		error = _status;
}
