
#ifndef VECTORDATA_H
#define VECTORDATA_H

#define VECTOR_RAM_SIZE 0x1000

class VectorData
{
public:
	VectorData(void);

	uint8_t	GetAt(uint16_t address);
	void		WriteVectorRAM(uint16_t address, uint8_t value);

private:
   uint8_t vectorRAM[VECTOR_RAM_SIZE];
};

#endif
