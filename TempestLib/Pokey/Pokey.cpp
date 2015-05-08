
#include "stdafx.h"

#include "TempestException.h"

#include "Pokey.h"


Pokey::Pokey(void)
{
	ALLPOT = 0;
}

Pokey::~Pokey(void)
{
}


uint8_t Pokey::ReadByte(uint16_t address) 
{
	switch (address)
	{
	case 0x8:
		return ALLPOT;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Invalid Pokey read: %02X", address);
			//throw TempestException(buf);
			return 0;
		}
	}
}

void Pokey::WriteByte(uint16_t address, uint8_t)
{
	switch (address)
	{
	case 0xB:
		// this just latches the "pot port state", which is basically just
		// a bunch of bit inputs
		ALLPOT = GetALLPOT();
		return;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Invalid Pokey write: %02X", address);
			//throw TempestException(buf);
		}
	}
}
