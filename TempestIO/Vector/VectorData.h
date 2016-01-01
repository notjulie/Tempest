
#ifndef VECTORDATA_H
#define VECTORDATA_H

#define COLOR_RAM_SIZE 0x10
#define VECTOR_RAM_SIZE 0x1000

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
