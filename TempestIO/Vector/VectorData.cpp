
#include "stdafx.h"

#include "../../TempestCPU/TempestROMS.h"

#include "VectorData.h"

static const uint16_t VECTOR_RAM_SIZE = 0x1000;
static const uint16_t VECTOR_ROM_SIZE = 0x1000;


VectorData::VectorData(void)
{
	vectorRAM.resize(VECTOR_RAM_SIZE);
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

uint8_t VectorData::ReadVectorRAM(uint16_t address)
{
	return vectorRAM[address];
}

void VectorData::WriteVectorRAM(uint16_t address, uint8_t value)
{
	vectorRAM[address] = value;
}
