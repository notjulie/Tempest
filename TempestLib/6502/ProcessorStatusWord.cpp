
#include "stdafx.h"

#include "CPU6502Exception.h"

#include "ProcessorStatusWord.h"


ProcessorStatusWord::ProcessorStatusWord(void)
{
	C = false;
	D = false;
	I = true;
	N = false;
	Z = false;
	overflowSetByDecimalMath = false;
}

bool ProcessorStatusWord::GetOverflow(void)
{
	// The 6502 overflow behavior is goofy in decimal mode; if a decimal mode
	// operation was the source of the current value of the overflow flag then
	// I want to throw an error; then I can scrutinize the code to make sure that
	// the overflow is being calculated the way the code expects it.

	// By the way, I doubt that this will occur... I don't see it as very
	// likely that someone would write something using the overflow result
	// of a decimal operation.  Did I mention that I've read that it also
	// varies between processor models?
	if (overflowSetByDecimalMath)
		throw CPU6502Exception("Overflow result of decimal math being used");

	return v;
}

void ProcessorStatusWord::ScrozzleOverflow(void)
{
	overflowSetByDecimalMath = true;
}

void ProcessorStatusWord::SetOverflow(bool value)
{
	v = value; overflowSetByDecimalMath = false;
}

uint8_t ProcessorStatusWord::ToByte(void) const
{
	uint8_t result = 0;
	if (C) result |= 0x01;
	if (D) result |= 0x08;
	if (I) result |= 0x04;
	if (N) result |= 0x80;
	if (v) result |= 0x40;
	if (Z) result |= 0x02;

	// hack
	if (overflowSetByDecimalMath) result |= 0x20;

	return result;
}

void ProcessorStatusWord::FromByte(uint8_t value)
{
	C = (value & 0x01) != 0;
	D = (value & 0x08) != 0;
	I = (value & 0x04) != 0;
	N = (value & 0x80) != 0;
	v = (value & 0x40) != 0;
	Z = (value & 0x02) != 0;

	// our hack
	overflowSetByDecimalMath = (value & 0x20) != 0;
}
