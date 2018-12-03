// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Emulation of the Vector State Machine; this reads the vector RAM
//    and converts it to a set of vectors, aka lines.
// ====================================================================

#include "TempestIO.Headers.h"
#include <stdarg.h>
#include <stdio.h>

#include "../../TempestCPU/TempestException.h"
#include "../TempestChar.h"

#include "VectorDataInterpreter.h"

VectorDataInterpreter::VectorDataInterpreter(void)
{
   for (int i = 0; i < 16 * 1024; ++i)
      hookFlags[i] = false;
}

VectorDataInterpreter::~VectorDataInterpreter(void)
{
}

void VectorDataInterpreter::Interpret(VectorGenerator *generator)
{
   // point at the generator
   this->vectorGenerator = generator;

   // start us out unhalted
   isHalt = false;

   // interpret starting at address zero
   InterpretAt(0);
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
      vectorGenerator->LDraw(x, y, vectorData.GetAt(pc + 3) >> 5);
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
         vectorGenerator->SDraw(x, y, vectorData.GetAt(pc) >> 5);
			return pc + 2;
		}

	case  6: // 0110
		//STAT
      vectorGenerator->Stat(
         vectorData.GetAt(pc) & 0xF,
         vectorData.GetAt(pc) >> 4,
         ReadColorRAM(vectorData.GetAt(pc) & 0xF)
         );
		return pc + 2;

	case  7: // 0111
		// SCALE
      vectorGenerator->Scale(vectorData.GetAt(pc + 1) & 7, vectorData.GetAt(pc));
		return pc + 2;

	case  8: // 1000
	case  9: // 1001
		// CENTER
      vectorGenerator->Center();
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


void VectorDataInterpreter::Char(TempestChar c)
{
   InterpretAt(GetCharSubroutineAddress(c));
}

uint16_t VectorDataInterpreter::GetCharSubroutineAddress(TempestChar c)
{
   if (c.ToAscii() == ' ')
      return 0x1168;
   else
      return 0x1000 + 2 * vectorData.GetAt(0x11e6 + 2 * c.GetRawValue());
}


/// <summary>
/// Implementation of printf that outputs characters to the current
/// XY position.  It is of course subject to the limitations of the
/// Tempest vector ROM's character set
/// </summary>
void VectorDataInterpreter::Printf(const char *format, ...)
{
   char buffer[256];
   va_list args;
   va_start(args, format);
   vsnprintf(buffer, sizeof(buffer), format, args);

   for (unsigned i = 0; i < sizeof(buffer); ++i)
   {
      if (buffer[i] == 0)
         break;
      Char(TempestChar::FromAscii(buffer[i]));
   }
}
