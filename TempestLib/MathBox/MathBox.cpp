
#include <stdint.h>
#include <stdio.h>

#include "../TempestException.h"

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
		throw TempestException("MathBox::LoadROM: invalid slot letter");
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
	// validate the address
	if (address >= romA.size())
	{
		char buffer[200];
		sprintf(buffer, "MathBox::Write unsupported address: 0x%X", address);
		SetError(buffer);
		return;
	}

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
	}
}

void MathBox::Update(void)
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
}

void MathBox::SetError(const std::string &_status)
{
	// we only record the first error
	if (error.size() == 0)
		error = _status;
}
