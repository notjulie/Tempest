
#include <stdio.h>

#include "TempestException.h"

#include "VectorGenerator.h"

static const uint16_t VECTOR_RAM_BASE = 0x2000;
static const uint16_t VECTOR_RAM_SIZE = 0x1000;
static const uint16_t VECTOR_ROM_BASE = 0x3000;
static const uint16_t VECTOR_ROM_SIZE = 0x1000;

VectorGenerator::VectorGenerator(void)
{
   vectorRAM.resize(VECTOR_RAM_SIZE);
   vectorROM.resize(VECTOR_ROM_SIZE);
}

void VectorGenerator::Go(void)
{
   // for now just process all vector commands to see if we can
   PC = 0;
   stack.resize(0);
   
   for (;;)
   {
      if (!SingleStep())
         break;
   }
   
   throw TempestException("Graphics output complete");
}

bool VectorGenerator::SingleStep(void)
{
   uint8_t opByte = GetAt(1);
   switch (opByte >> 4)
   {
      case  0: // 0000
      case  1: // 0001
         printf("LDRAW (%d,%d); %d\n",
            (GetAt(2) + 256 * GetAt(3)) & 0x1FFF,
            (GetAt(0) + 256 * GetAt(1)) & 0x1FFF,
            GetAt(3) >> 5
            );
         PC += 4;
         return true;
         
      case  2: // 0010
      case  3: // 0011
         return false;

      case  4: // 0100         
      case  5: // 0101
      {
         int x = GetAt(0) & 0x0F;
         if ((GetAt(0) & 0x10) != 0)
            x = -16 + x;
         int y = GetAt(1) & 0x0F;
         if ((GetAt(1) & 0x10) != 0)
            y = -16 + y;
         printf("SDRAW (%d,%d); %d\n",
            x,
            y,
            GetAt(0) >> 5
            );
         PC += 2;
         return true;
      }

      case  6: // 0110
         printf("STAT %d, %d\n",
            GetAt(0) & 0xF,
            GetAt(0) >> 4
            );
         PC += 2;
         return true;
         
      case  7: // 0111
         printf("SCALE %d,%d\n",
            GetAt(1) & 7,
            GetAt(0)
            );
         PC += 2;
         return true;
         
      case  8: // 1000
      case  9: // 1001
         printf("CENTER\n");
         PC += 2;
         return true;

      case 10: // 1010
      case 11: // 1011
         printf("JSR %X\n",
            2 * ((GetAt(0) + 256 * GetAt(1)) & 0x1FFF)
            );
         stack.push_back(PC + 2);
         PC = 2 * ((GetAt(0) + 256 * GetAt(1)) & 0x1FFF);
         return true;

      case 12: // 1100
      case 13: // 1101
         printf("RTS\n");
         PC = stack[stack.size() - 1];
         stack.pop_back();
         return true;
         
      case 14: // 1110
      case 15: // 1111
         throw TempestException("JUMP op not implemented");
   }
}


uint8_t VectorGenerator::GetAt(uint16_t pcOffset)
{
   uint16_t address = PC + pcOffset;
   if (address >= VECTOR_RAM_SIZE)
      return vectorROM[address - VECTOR_RAM_SIZE];
   else
      return vectorRAM[address];
}



bool VectorGenerator::IsVectorRAMAddress(uint16_t address) const
{
   return address>=VECTOR_RAM_BASE && address<VECTOR_RAM_BASE+VECTOR_RAM_SIZE;
}


bool VectorGenerator::IsVectorROMAddress(uint16_t address) const
{
   return address>=VECTOR_ROM_BASE && address<VECTOR_ROM_BASE+VECTOR_ROM_SIZE;
}

void VectorGenerator::LoadROM(uint16_t address,const uint8_t *buffer, int count)
{
   for (int i=0; i<count; ++i)
      vectorROM[address - VECTOR_ROM_BASE + i] = buffer[i];
}

uint8_t VectorGenerator::ReadVectorRAM(uint16_t address)
{
   return vectorRAM[address - VECTOR_RAM_BASE];
}

uint8_t VectorGenerator::ReadVectorROM(uint16_t address)
{
   return vectorROM[address - VECTOR_ROM_BASE];
}

void VectorGenerator::Reset(void)
{
   printf("Vector generator reset\n");
}


void VectorGenerator::WriteVectorRAM(uint16_t address, uint8_t value)
{
   vectorRAM[address - VECTOR_RAM_BASE] = value;
}
