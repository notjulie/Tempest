
#include "stdafx.h"

#include "../../TempestCPU/TempestROMS.h"

#include "VectorData.h"



VectorData::VectorData(void)
{
   // start out with a HALT as the first instruction
   vectorRAM[1] = 0x20;
}


uint8_t VectorData::GetAt(uint16_t address)
{
	if (address < VECTOR_RAM_SIZE)
		return vectorRAM[address];
	else if (address < VECTOR_RAM_SIZE + sizeof(ROM_136002_111))
		return ROM_136002_111[address - VECTOR_RAM_SIZE];
	else
		return ROM_136002_112[address - VECTOR_RAM_SIZE - sizeof(ROM_136002_111)];
}

void VectorData::WriteVectorRAM(uint16_t address, uint8_t value)
{
   vectorRAM[address] = value;
}

void VectorData::WriteColorRAM(uint16_t address, uint8_t value)
{
   // color RAM is only 4 bits
   colorRAM[address] = value & 0x0F;
}
