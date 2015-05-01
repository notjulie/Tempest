
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

/*	// validate the address
	if (address >= romA.size())
	{
		char buffer[200];
		sprintf(buffer, "MathBox::Write unsupported address: 0x%X", address);
		SetError(buffer);
		return;
	}



	// On a write to the MathBox, the 109 JK at A2 causes the BEGIN signal
	// to go active during the high cycle of the 3MHz clock... BEGIN remains
	// active until the write cycle completes.

	// The result of BEGIN going active will be that the D flip-flop at D5 will
	// be cleared, enabling the Math Box clock, whose first edge will be a rising edge
	// when the 3MHz clock goes low.

	// When A12 goes low, the Math Box clock will be disabled on its next falling edge.


	// So when BEGIN is activated:
	//   - the ROM at A1 puts the start address on the bus going to the counters
	//   - the PCEN is activated, putting the counters into "load" mode
	//   - the 
	// the value from the ROM becomes the new program counter
	pc = romA[address];

	// the value being written is the data value
	dataIn = value;

	// we have all the outside inputs accounted for... update
	try
	{
		Update();
	}
	catch (TempestException &_x)
	{
		SetError(_x.what());
	}*/
}


void MathBox::HandleRisingClock(void)
{
	throw MathBoxException("MathBox::HandleRisingClock not implemented");
}

void MathBox::HandleFallingClock(void)
{
	throw MathBoxException("MathBox::HandleFallingClock not implemented");
}

/*void MathBox::Update(void)
{
	SetError("D5 not being calculated");

	// the A & B inputs to the ALUs are all the same
	aluK.A = aluF.A = aluJ.A = aluE.A = romL[pc];
	aluK.B = aluF.B = aluJ.B = aluE.B = romK[pc];

	// calculate A10star
	bool M = (romE[pc] & 2) != 0;
	bool E4 = M & !D5;
	bool A10 = (romJ[pc] & 2) != 0;
	bool A10star = A10 ^ E4;

	// I8-I0 come from these pins on the ALU
	// 6    7 5 27 28    26 14 13 12
	// I8-I3 are easy
	uint16_t I = ((romF[pc] & 7) << 6) + ((romH[pc] & 7) << 3);
	if (A10star)
		I |= 2;
	if (romJ[pc] & 1)
		I |= 1;
	aluK.I = aluF.I = I + (romJ[pc] & 4);
	aluJ.I = aluE.I = I + ((romJ[pc] & 8) >> 1);

	bool A12 = (romH[pc] & 8) != 0;
	bool LDAB = (romF[pc] & 8) != 0;

	// update the ALUs
	SetError("Inputs to ALUs not set");
	aluK.CarryIn = (romE[pc] & 1) != 0;
	aluK.Update();

	aluF.CarryIn = aluK.CarryOut;
	aluF.Update();

	aluJ.CarryIn = aluF.CarryOut;
	aluJ.Update();

	aluE.CarryIn = aluJ.CarryOut;
	aluE.Update();

	SetError("Iteration not implemented");
}*/

void MathBox::SetError(const std::string &_status)
{
	// we only record the first error
	if (error.size() == 0)
		error = _status;
}
