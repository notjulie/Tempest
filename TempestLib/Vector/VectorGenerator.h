/*
 * File:   VectorGenerator.h
 * Author: Randy
 *
 * Created on April 26, 2015, 12:42 PM
 */

#ifndef VECTORGENERATOR_H
#define	VECTORGENERATOR_H

#include "VectorData.h"

class VectorGenerator
{
public:
	VectorGenerator(void);

	void		Go(void);
	bool	   HaveNewData(void);
	bool		IsVectorRAMAddress(uint16_t address) const;
	bool		IsVectorROMAddress(uint16_t address) const;
	void		LoadROM(uint16_t address, const uint8_t *buffer, int count);
	void		Pop(VectorData &_vectorData);
	uint8_t	ReadVectorRAM(uint16_t address);
	uint8_t	ReadVectorROM(uint16_t address);
	void		Reset(void);
	void		WriteVectorRAM(uint16_t address, uint8_t value);

private:
	VectorData	vectorData;
	bool haveNewData;
};

#endif	/* VECTORGENERATOR_H */

