
#include <stdint.h>
#include <stdio.h>

#include "TempestException.h"

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
   char buffer[200];
   sprintf(buffer, "MathBox::Write unsupported address: 0x%X", address);
   SetError(buffer);
}

void MathBox::SetError(const std::string &_status)
{
	// we only record the first error
	if (error.size() == 0)
		error = _status;
}
