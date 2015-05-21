
#include "stdafx.h"

#include "../../TempestCPU/TempestException.h"

#include "VectorGenerator.h"

static const uint16_t VECTOR_RAM_BASE = 0x2000;
static const uint16_t VECTOR_ROM_BASE = 0x3000;

VectorGenerator::VectorGenerator(void)
{
	haveNewData = false;
}

void VectorGenerator::Go(void)
{
	haveNewData = true;
}


bool VectorGenerator::IsHalt(void)
{
	// for now just return true every 10th time we're asked... that's a frame
	// update rate of about 18Hz.
	return (++haltCycle % 10) == 0;
}

void VectorGenerator::Pop(VectorData &_vectorData)
{
	_vectorData = vectorData;
	haveNewData = false;
}


bool VectorGenerator::HaveNewData(void)
{
	return haveNewData;
}


uint8_t VectorGenerator::ReadVectorRAM(uint16_t address)
{
	return vectorData.ReadVectorRAM((uint16_t)(address - VECTOR_RAM_BASE));
}

uint8_t VectorGenerator::ReadVectorROM(uint16_t address)
{
	return vectorData.ReadVectorROM((uint16_t)(address - VECTOR_ROM_BASE));
}

void VectorGenerator::Reset(void)
{
}


void VectorGenerator::WriteVectorRAM(uint16_t address, uint8_t value)
{
	vectorData.WriteVectorRAM((uint16_t)(address - VECTOR_RAM_BASE), value);
}
