
#include <stdio.h>

#include "../TempestException.h"

#include "VectorGenerator.h"

static const uint16_t VECTOR_RAM_BASE = 0x2000;
static const uint16_t VECTOR_ROM_BASE = 0x3000;

VectorGenerator::VectorGenerator(void)
{
}

void VectorGenerator::Go(void)
{
	haveNewData = true;

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

void VectorGenerator::Pop(VectorData &_vectorData)
{
	_vectorData = vectorData;
	haveNewData = false;
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

bool VectorGenerator::HaveNewData(void)
{
	return haveNewData;
}

uint8_t VectorGenerator::GetAt(uint16_t pcOffset)
{
   return vectorData.GetAt(PC + pcOffset);
}



bool VectorGenerator::IsVectorRAMAddress(uint16_t address) const
{
   return address>=VECTOR_RAM_BASE && address<VECTOR_RAM_BASE+VectorData::GetVectorRAMSize();
}


bool VectorGenerator::IsVectorROMAddress(uint16_t address) const
{
	return address >= VECTOR_ROM_BASE && address<VECTOR_ROM_BASE + VectorData::GetVectorROMSize();
}

void VectorGenerator::LoadROM(uint16_t address,const uint8_t *buffer, int count)
{
	vectorData.LoadROM(address - VECTOR_ROM_BASE, buffer, count);
}

uint8_t VectorGenerator::ReadVectorRAM(uint16_t address)
{
   return vectorData.ReadVectorRAM(address - VECTOR_RAM_BASE);
}

uint8_t VectorGenerator::ReadVectorROM(uint16_t address)
{
   return vectorData.ReadVectorROM(address - VECTOR_ROM_BASE);
}

void VectorGenerator::Reset(void)
{
   printf("Vector generator reset\n");
}


void VectorGenerator::WriteVectorRAM(uint16_t address, uint8_t value)
{
   vectorData.WriteVectorRAM(address - VECTOR_RAM_BASE, value);
}
