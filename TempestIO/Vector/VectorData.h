
#ifndef VECTORDATA_H
#define VECTORDATA_H


class VectorData
{
public:
	VectorData(void);

	uint8_t	GetAt(uint16_t address);
	uint8_t	ReadVectorRAM(uint16_t address);
	uint8_t	ReadVectorROM(uint16_t address);
	void		WriteVectorRAM(uint16_t address, uint8_t value);

private:
	std::vector<uint8_t>  vectorRAM;
};

#endif
