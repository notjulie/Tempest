
#include "VectorData.h"

static const uint16_t VECTOR_RAM_SIZE = 0x1000;
static const uint16_t VECTOR_ROM_SIZE = 0x1000;


VectorData::VectorData(void)
{
	vectorRAM.resize(VECTOR_RAM_SIZE);
	vectorROM.resize(VECTOR_ROM_SIZE);
}


uint8_t VectorData::GetAt(uint16_t address)
{
	if (address >= VECTOR_RAM_SIZE)
		return vectorROM[address - VECTOR_RAM_SIZE];
	else
		return vectorRAM[address];
}

uint16_t VectorData::GetVectorRAMSize(void)
{
	return VECTOR_RAM_SIZE;
}

uint16_t VectorData::GetVectorROMSize(void)
{
	return VECTOR_ROM_SIZE;
}

void VectorData::LoadROM(uint16_t address, const uint8_t *buffer, int count)
{
	for (int i = 0; i<count; ++i)
		vectorROM[address + i] = buffer[i];
}

uint8_t VectorData::ReadVectorRAM(uint16_t address)
{
	return vectorRAM[address];
}

uint8_t VectorData::ReadVectorROM(uint16_t address)
{
	return vectorROM[address];
}

void VectorData::WriteVectorRAM(uint16_t address, uint8_t value)
{
	vectorRAM[address] = value;
}
