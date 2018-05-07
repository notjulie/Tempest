
#include "stdafx.h"

#include "TempestCPU/TempestException.h"
#include "AsteroidsROMs.h"

#include "AsteroidsVectorInterpreter.h"

static int ShortVectorLength(int code);

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

static int lastJSR;

bool AsteroidsVectorInterpreter::SingleStep(void)
{
   // process reset if we have one
   if (resetRequested)
   {
      PC = 0;
      stackIndex = 0;
      isHalt = true;
      resetRequested = false;
      globalScale = 0;
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
   uint8_t opCode = opByte >> 4;
   switch (opCode)
   {
   case 0x8:
   case 0x9:
   {
      // LDRAW
      int rawX = (GetAt(2) + 256 * GetAt(3)) & 0x0FFF;
      int rawY = (GetAt(0) + 256 * GetAt(1)) & 0x0FFF;

      int dx = (GetAt(2) + 256 * GetAt(3)) & 0x07FF;
      if (dx & 0x400)
         dx = -(dx & ~0x400);
      int dy = (GetAt(0) + 256 * GetAt(1)) & 0x07FF;
      if (dy & 0x400)
         dy = -(dy & ~0x400);

      int scale = 3;
      if (opCode == 9)
         scale = GetAt(3) >> 4;
      dx = dx * (scale + 1) / 8;
      dy = dy * (scale + 1) / 8;
      //Draw(dx, dy, 7, 1);

      PC += 4;
      return true;
   }

   case 0xA:
      // go to absolute XY position
      {
         int newX = (GetAt(2) + 256 * GetAt(3)) & 0x07FF;
         if (newX & 0x400)
            newX = -(newX & ~0x400);
         int newY = (GetAt(0) + 256 * GetAt(1)) & 0x07FF;
         if (newY & 0x400)
            newY = -(newY & ~0x400);

         x = (float)newX;
         y = (float)newY;

         globalScale = GetAt(3) >> 4;

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
      lastJSR = PC;
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
      // short vector draw
      {
         int raw1 = GetAt(1);
         int raw0 = GetAt(0);

         int dx = (GetAt(0) & 0x07);
         if (dx & 0x4)
            dx = -(dx & ~0x4);
         else
            dx += 0;
         int dy = (GetAt(1) & 0x07);
         if (dy & 0x4)
            dy = -(dy & ~0x4);
         else
            dy += 0;

         if (dx == dy)
            dx = dy;

         int scale = 1;
         if (raw1 & 0x08)
            scale *= 2;
         if (raw0 & 0x08)
            scale *= 4;

         int scaleMultiplier = 4;
         uint8_t intensity = GetAt(0) >> 4;
         bool curiousBit = (raw0 & 0x80) != 0;


         Draw(dx * scaleMultiplier * (scale), dy * scaleMultiplier * (scale), intensity, 1);
         PC += 2;
      }
      return true;

   case 0x0:
   case 0x1:
   case 0x2:
   case 0x3:
   case 0x4:
   case 0x5:
   case 0x6:
   case 0x7:
      // I think these are scale commands, but I'm not sure
      //scale = opCode;
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


void AsteroidsVectorInterpreter::Draw(int _dx, int _dy, uint8_t intensity, int scaleFactor)
{
   float dx = (float)_dx;
   float dy = (float)_dy;

   // apply the global scale
   if (globalScale >= 8)
   {
      dx /= 1 << (16 - globalScale);
      dy /= 1 << (16 - globalScale);
   }
   else
   {
      dx *= 1 << globalScale;
      dy *= 1 << globalScale;
   }

   int scale = 64;
   SimpleVector vector;
   vector.startX = (int16_t)(-32768 + scale*x);
   vector.startY = (int16_t)(-32768 + scale*y);
   x += dx;
   y += dy;
   vector.endX = (int16_t)(-32768 + scale*x);
   vector.endY = (int16_t)(-32768 + scale*y);
   vector.color = 1;

   if (intensity != 0)
      vectors.push_back(vector);
}

static int ShortVectorLength(int code)
{
   int result = 0;
   if (code & 8)
      result += 2;
   if (code & 4)
      result += 4;
   if (code & 2)
      result += 8;
   if (code & 1)
      result = -16 + result;
   return result;
}
