
#ifndef VECTORDATA_H
#define VECTORDATA_H

#include <stdint.h>
#include <vector>

class VectorData
{
public:
	VectorData(void);

	uint8_t	GetAt(uint16_t address);
	void		LoadROM(uint16_t address, const uint8_t *buffer, int count);
	uint8_t	ReadVectorRAM(uint16_t address);
	uint8_t	ReadVectorROM(uint16_t address);
	void		WriteVectorRAM(uint16_t address, uint8_t value);

public:
	static uint16_t GetVectorRAMSize(void);
	static uint16_t GetVectorROMSize(void);

private:
	std::vector<uint8_t>  vectorRAM;
	std::vector<uint8_t>  vectorROM;
};

#endif
