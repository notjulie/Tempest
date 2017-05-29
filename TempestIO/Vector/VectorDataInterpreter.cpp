
#include "stdafx.h"

#include "../../TempestCPU/TempestException.h"

#include "VectorDataInterpreter.h"

VectorDataInterpreter::VectorDataInterpreter(void)
{
   for (int i = 0; i < 16 * 1024; ++i)
      hookFlags[i] = false;
}

VectorDataInterpreter::~VectorDataInterpreter(void)
{
}

void VectorDataInterpreter::InterpretAt(uint16_t pc)
{
   while (!isHalt)
	{
      // see if there's a hook
      if (hookFlags[pc])
      {
         pc = hooks[pc](pc);
         continue;
      }

      // else just single step it
      pc = SingleStep(pc);
      if (pc == 0)
         break;
	}
}


uint16_t VectorDataInterpreter::SingleStep(uint16_t pc)
{
	if (isHalt)
		return 0;

	uint8_t opByte = vectorData.GetAt(pc + 1);
	switch (opByte >> 4)
	{
	case  0: // 0000
	case  1: // 0001
	{
		// LDRAW
      int x = (vectorData.GetAt(pc + 2) + 256 * vectorData.GetAt(pc + 3)) & 0x1FFF;
		if (x & 0x1000)
			x = -0x1000 + (x & ~0x1000);
      int y = (vectorData.GetAt(pc) + 256 * vectorData.GetAt(pc + 1)) & 0x1FFF;
		if (y & 0x1000)
			y = -0x1000 + (y & ~0x1000);
      LDraw(x, y, vectorData.GetAt(pc + 3) >> 5);
		return pc + 4;
	}

	case  2: // 0010
	case  3: // 0011
		// HALT
		isHalt = true;
		return 0;

	case  4: // 0100         
	case  5: // 0101
		// SDRAW
		{
         int x = vectorData.GetAt(pc) & 0x0F;
         if ((vectorData.GetAt(pc) & 0x10) != 0)
				x = -16 + x;
         int y = vectorData.GetAt(pc + 1) & 0x0F;
         if ((vectorData.GetAt(pc + 1) & 0x10) != 0)
				y = -16 + y;
         SDraw(x, y, vectorData.GetAt(pc) >> 5);
			return pc + 2;
		}

	case  6: // 0110
		//STAT
      Stat(vectorData.GetAt(pc) & 0xF, vectorData.GetAt(pc) >> 4);
		return pc + 2;

	case  7: // 0111
		// SCALE
      Scale(vectorData.GetAt(pc + 1) & 7, vectorData.GetAt(pc));
		return pc + 2;

	case  8: // 1000
	case  9: // 1001
		// CENTER
		Center();
		return pc + 2;

	case 10: // 1010
	case 11: // 1011
		// JSR
      InterpretAt((uint16_t)(2 * ((vectorData.GetAt(pc) + 256 * vectorData.GetAt(pc + 1)) & 0x1FFF)));
		return isHalt ? 0 : pc + 2;

	case 12: // 1100
	case 13: // 1101
		// RTS
		return 0;

	case 14: // 1110
	case 15: // 1111
		// JUMP
      return (uint16_t)(2 * ((vectorData.GetAt(pc) + 256 * vectorData.GetAt(pc + 1)) & 0x1FFF));

	default:
		throw TempestException("VectorDataInterpreter bad op code");
	}
}



void VectorDataInterpreter::RegisterHook(uint16_t address, std::function<uint16_t(uint16_t)> hook)
{
   // Just to save on confusion I allow the caller to specicfy the address as either
   // a bus address or as a relative offset in the vector memory space.  Just strip off
   // the bits we don't need.
   address &= 0x1FFF;

   hookFlags[address] = true;
   hooks[address] = hook;
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

