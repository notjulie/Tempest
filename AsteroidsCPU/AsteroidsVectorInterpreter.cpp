
#include "stdafx.h"
#include <memory.h>

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
   case 0x0:
   case 0x1:
   case 0x2:
   case 0x3:
   case 0x4:
   case 0x5:
   case 0x6:
   case 0x7:
   case 0x8:
   case 0x9:
   {
      // LDRAW
      int dx = (GetAt(2) + 256 * GetAt(3)) & 0x07FF;
      if (dx & 0x400)
         dx = -(dx & ~0x400);
      int dy = (GetAt(0) + 256 * GetAt(1)) & 0x07FF;
      if (dy & 0x400)
         dy = -(dy & ~0x400);

      int intensity = GetAt(3) >> 4;

      Draw(dx, dy, intensity, 1<<(9 - opCode));

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

         int shift = 1;
         if (raw1 & 0x08)
            shift += 1;
         if (raw0 & 0x08)
            shift += 2;

         uint8_t intensity = GetAt(0) >> 4;


         Draw(dx * (1 << shift), dy * (1 << shift), intensity, 1);
         PC += 2;
      }
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

   // apply the scale factor
   dx /= scaleFactor;
   dy /= scaleFactor;

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

   // calculate the line endpoints extended to 32 bit
   int scale = 64;
   int startX = (int)(-32768 + scale * x);
   int startY = (int)(-32768 + scale * y);
   x += dx;
   y += dy;
   int endX = (int)(-32768 + scale * x);
   int endY = (int)(-32768 + scale * y);

   // we need to clip the line to the bounds of signed 16 bits
   if (!ClipEndPoint(startX, startY, endX, endY))
      return;
   if (!ClipEndPoint(endX, endY, startX, startY))
      return;

   SimpleVector vector;
   vector.startX = (int16_t)startX;
   vector.startY = (int16_t)startY;
   vector.endX = (int16_t)endX;
   vector.endY = (int16_t)endY;
   vector.r = vector.g = vector.b = 0xFF;

   if (intensity != 0)
      vectors.push_back(vector);
}

bool AsteroidsVectorInterpreter::ClipEndPoint(int &startX, int &startY, int &endX, int &endY)
{
   if (endX < -32768)
   {
      if (startX < -32768)
         return false;
      endY = (int)(startY + (int64_t)(endY - startY) * (-32768 - startX) / (endX - startX));
      endX = -32768;
   }

   if (endY < -32768)
   {
      if (startY < -32768)
         return false;
      endX = (int)(startX + (int64_t)(endX - startX) * (-32768 - startY) / (endY - startY));
      endY = -32768;
   }

   if (endX > 32767)
   {
      if (startX > 32767)
         return false;
      endY = (int)(startY + (int64_t)(endY - startY) * (32767 - startX) / (endX - startX));
      endX = 32767;
   }

   if (endY > 32767)
   {
      if (startY > 32767)
         return false;
      endX = (int)(startX + (int64_t)(endX - startX) * (32767 - startY) / (endY - startY));
      endY = 32767;
   }

   return true;
}

