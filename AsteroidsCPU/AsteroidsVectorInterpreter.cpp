
#include "stdafx.h"

#include "TempestCPU/TempestException.h"
#include "AsteroidsROMs.h"

#include "AsteroidsVectorInterpreter.h"


void AsteroidsVectorInterpreter::SetVectorRAM(const void *vectorRAM)
{
   memcpy(this->vectorRAM, vectorRAM, 0x800);
}

void AsteroidsVectorInterpreter::Interpret(void)
{
   resetRequested = true;
   goRequested = true;
   vectors.resize(0);

   for (;;)
   {
      if (!SingleStep())
         break;
   }
}

void AsteroidsVectorInterpreter::GetAllVectors(std::vector<SimpleVector> &vectors)
{
   vectors = this->vectors;
}

bool AsteroidsVectorInterpreter::SingleStep(void)
{
   // process reset if we have one
   if (resetRequested)
   {
      PC = 0;
      stackIndex = 0;
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
   case 0x8:
   case 0x9:
   {
      // LDRAW
      int dx = (GetAt(2) + 256 * GetAt(3)) & 0x0FFF;
      if (dx & 0x800)
         dx = -0x800 + (dx & ~0x800);
      int dy = (GetAt(0) + 256 * GetAt(1)) & 0x0FFF;
      if (dy & 0x800)
         dy = -0x800 + (dy & ~0x800);
      Draw(dx, dy);
      PC += 4;
      return true;
   }

   case 0xA:
      // go to absolute XY position
      {
         x = (GetAt(2) + 256 * GetAt(3)) & 0x0FFF;
         if (x & 0x0800)
            x = -0x0800 + (x & ~0x0800);
         y = (GetAt(0) + 256 * GetAt(1)) & 0x0FFF;
         if (y & 0x0800)
            y = -0x0800 + (y & ~0x0800);
         PC += 4;
         return true;
      }

   case 0xB:
      // HALT
      isHalt = true;
      return false;

   case 0xC:
      // JSR
      stack[stackIndex++] = (uint16_t)(PC + 2);
      PC = (uint16_t)(2 * ((GetAt(0) + 256 * GetAt(1)) & 0x0FFF));
      return PC != 0;

   case 0xD:
      // RTS
      if (stackIndex == 0)
         return false;
      PC = stack[--stackIndex];
      return true;

   case 0xE:
      // JUMP
      PC = (uint16_t)(2 * ((GetAt(0) + 256 * GetAt(1)) & 0x0FFF));

      // a jump to address zero basically means we are done and starting over,
      // but not halted
      return PC != 0;

   case 0xF:
      {
         int dx = GetAt(0) & 0x07;
         if ((GetAt(0) & 0x08) != 0)
            dx = -8 + dx;
         int dy = GetAt(1) & 0x07;
         if ((GetAt(1) & 0x08) != 0)
            dy = -8 + dy;
         Draw(dx, dy);
         PC += 2;
         return true;
      }


   case 0x0:
   case 0x1:
   case 0x2:
   case 0x3:
   case 0x4:
   case 0x5:
   case 0x6:
   case 0x7:
      // I think these are scale commands, but I'm not sure
      PC += 2;
      return true;

   default:
      throw TempestException("VectorDataInterpreter bad op code");
   }
}

uint8_t AsteroidsVectorInterpreter::GetAt(uint16_t pcOffset)
{
   uint16_t address = PC + pcOffset;
   if (address>=0 && address < 0x800)
      return vectorRAM[address];
   else if (address>=0x1000 && address<0x2000)
      return ROM035127_02[address - 0x1000];
   else
      throw TempestException("AsteroidsVectorInterpreter bad address");
}


void AsteroidsVectorInterpreter::Draw(int dx, int dy)
{
   int scale = 50;
   SimpleVector vector;
   vector.startX = scale*x;
   vector.startY = scale*y;
   x += dx;
   y += dy;
   vector.endX = scale*x;
   vector.endY = scale*y;
   vector.color = 1;
   vectors.push_back(vector);
}

