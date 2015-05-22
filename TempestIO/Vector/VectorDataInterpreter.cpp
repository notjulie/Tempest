
#include "stdafx.h"

#include "../../TempestCPU/TempestException.h"

#include "VectorDataInterpreter.h"

VectorDataInterpreter::VectorDataInterpreter(void)
{
	isHalt = true;
	resetRequested = true;
	goRequested = false;
	PC = 0;
}

VectorDataInterpreter::~VectorDataInterpreter(void)
{
}

void VectorDataInterpreter::Reset(void)
{
	isHalt = true;
	resetRequested = true;
}

void VectorDataInterpreter::Interpret(void)
{
	for (;;)
	{
		if (!SingleStep())
			break;
	}
}


bool VectorDataInterpreter::SingleStep(void)
{
	// process reset if we have one
	if (resetRequested)
	{
		PC = 0;
		stack.resize(0);
		isHalt = true;
		resetRequested = false;
	}

	// process go if we have one
	if (goRequested)
	{
		isHalt = false;
		goRequested = false;
	}

	if (isHalt)
		return false;

	uint8_t opByte = GetAt(1);
	switch (opByte >> 4)
	{
	case  0: // 0000
	case  1: // 0001
	{
		// LDRAW
		int x = (GetAt(2) + 256 * GetAt(3)) & 0x1FFF;
		if (x & 0x1000)
			x = -0x1000 + (x & ~0x1000);
		int y = (GetAt(0) + 256 * GetAt(1)) & 0x1FFF;
		if (y & 0x1000)
			y = -0x1000 + (y & ~0x1000);
		LDraw(x/2, y/2, GetAt(3) >> 5);
		PC += 4;
		return true;
	}

	case  2: // 0010
	case  3: // 0011
		// HALT
		isHalt = true;
		return false;

	case  4: // 0100         
	case  5: // 0101
		// SDRAW
		{
			int x = GetAt(0) & 0x0F;
			if ((GetAt(0) & 0x10) != 0)
				x = -16 + x;
			int y = GetAt(1) & 0x0F;
			if ((GetAt(1) & 0x10) != 0)
				y = -16 + y;
			SDraw(x,	y,	GetAt(0) >> 5);
			PC += 2;
			return true;
		}

	case  6: // 0110
		//STAT
		Stat(GetAt(0) & 0xF, GetAt(0) >> 4);
		PC += 2;
		return true;

	case  7: // 0111
		// SCALE
		Scale(GetAt(1) & 7, GetAt(0));
		PC += 2;
		return true;

	case  8: // 1000
	case  9: // 1001
		// CENTER
		Center();
		PC += 2;
		return true;

	case 10: // 1010
	case 11: // 1011
		// JSR
		stack.push_back((uint16_t)(PC + 2));
		PC = (uint16_t)(2 * ((GetAt(0) + 256 * GetAt(1)) & 0x1FFF));
		return true;

	case 12: // 1100
	case 13: // 1101
		// RTS
		if (stack.size() == 0)
			return false;
		PC = stack[stack.size() - 1];
		stack.pop_back();
		return true;

	case 14: // 1110
	case 15: // 1111
		// JUMP
		PC = (uint16_t)(2 * ((GetAt(0) + 256 * GetAt(1)) & 0x1FFF));

		// a jump to address zero basically means we are done and starting over,
		// but not halted
		return PC != 0;

	default:
		throw TempestException("VectorDataInterpreter bad op code");
	}
}


uint8_t VectorDataInterpreter::GetAt(uint16_t pcOffset)
{
	return vectorData.GetAt((uint16_t)(PC + pcOffset));
}


void VectorDataInterpreter::Center(void)
{
}

void VectorDataInterpreter::LDraw(int, int, int)
{
}

void VectorDataInterpreter::Scale(int, int)
{
}

void VectorDataInterpreter::SDraw(int, int, int)
{
}

void VectorDataInterpreter::Stat(int, int)
{
}

