
#include <stdint.h>
#include <stdio.h>

#include "MathBoxException.h"

#include "MathBox.h"


void MathBox::LoadROM(const uint8_t *rom, int length, char slot)
{
	switch (slot)
	{
	case 'A':
		romA.resize(length);
		memcpy(&romA[0], rom, length);
		break;

	case 'E':
		romE.resize(length);
		memcpy(&romE[0], rom, length);
		break;

	case 'F':
		romF.resize(length);
		memcpy(&romF[0], rom, length);
		break;

	case 'H':
		romH.resize(length);
		memcpy(&romH[0], rom, length);
		break;

	case 'J':
		romJ.resize(length);
		memcpy(&romJ[0], rom, length);
		break;

	case 'K':
		romK.resize(length);
		memcpy(&romK[0], rom, length);
		break;

	case 'L':
		romL.resize(length);
		memcpy(&romL[0], rom, length);
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
			newPC = romA[addressIn];
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

	// let the ALUs handle the rising clock edge...
	SetALUInputs();
	aluK.SetClock(true);
	aluF.SetClock(true);
	aluJ.SetClock(true);
	aluE.SetClock(true);

	// latch all the state values that we are supposed to latch on the
	// rising clock
	PC = newPC;
}

void MathBox::HandleFallingClock(void)
{
	bool newSTOP;
	throw MathBoxException("MathBox::HandleFallingClock: not calculating STOP");

	// latch all the state values that we are supposed to latch on the
	// falling clock
	STOP = newSTOP;

	throw MathBoxException("MathBox::HandleFallingClock needs to update the ALUs");
}

bool MathBox::GetBit(Bit bit)
{
	char buf[200];

	switch (bit)
	{
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

	default:
		sprintf(buf, "MathBox::GetBit: unsupported bit: %d", bit);
		throw MathBoxException(buf);
	}
}

void MathBox::SetALUInputs(void)
{
	// if the PC is not set our ROM values are also considered unknown...
	// just a diagnostic measure
	if (PC < 0)
	{
		aluK.AAddress = aluF.AAddress = aluJ.AAddress = aluE.AAddress = -1;
		aluK.BAddress = aluF.BAddress = aluJ.BAddress = aluE.BAddress = -1;
		aluK.I012 = aluF.I012 = aluJ.I012 = aluE.I012 = -1;
		aluK.I345 = aluF.I345 = aluJ.I345 = aluE.I345 = -1;
		aluK.I678 = aluF.I678 = aluJ.I678 = aluE.I678 = -1;
	}
	else
	{
		// the A & B inputs to the ALUs are all the same
		aluK.AAddress = aluF.AAddress = aluJ.AAddress = aluE.AAddress = romL[PC];
		aluK.BAddress = aluF.BAddress = aluJ.BAddress = aluE.BAddress = romK[PC];
		aluK.I345 = aluF.I345 = aluJ.I345 = aluE.I345 = romH[PC] & 7;
		aluK.I678 = aluF.I678 = aluJ.I678 = aluE.I678 = romF[PC] & 7;

		int i01 = romJ[PC] & 1;
		if (GetBit(A10STAR))
			i01 += 2;
		aluK.I012 = aluF.I012 = i01 + (romJ[PC] & 4);
		aluJ.I012 = aluE.I012 = i01 + ((romJ[PC] & 8) >> 1);
	}

	if (dataIn < 0)
	{
		aluK.DataIn = aluF.DataIn = aluJ.DataIn = aluE.DataIn = -1;
	}
	else
	{
		aluK.DataIn = aluJ.DataIn = dataIn & 0xF;
		aluF.DataIn = aluE.DataIn = dataIn >> 4;
	}

	aluK.RAM0 = GetBit(R0);
	aluK.Q0 = GetBit(Q0);
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
