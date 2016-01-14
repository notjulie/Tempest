
#ifndef VECTORDATA_H
#define VECTORDATA_H

#include "../../TempestCPU/TempestMemoryMap.h"

class VectorData
{
public:
	VectorData(void);

	uint8_t	GetAt(uint16_t address);
   uint8_t  ReadColorRAM(uint16_t address) { return colorRAM[address]; }
   void		WriteColorRAM(uint16_t address, uint8_t value);
   void		WriteVectorRAM(uint16_t address, uint8_t value);

private:
   uint8_t colorRAM[COLOR_RAM_SIZE];
   uint8_t vectorRAM[VECTOR_RAM_SIZE];
};

#endif
