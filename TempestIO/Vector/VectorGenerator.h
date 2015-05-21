/*
 * File:   VectorGenerator.h
 * Author: Randy
 *
 * Created on April 26, 2015, 12:42 PM
 */

#ifndef VECTORGENERATOR_H
#define	VECTORGENERATOR_H

#include "VectorData.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class VectorGenerator
{
public:
	VectorGenerator(void);

	void		Go(void);
	bool	   HaveNewData(void);
	bool		IsHalt(void);
	void		Pop(VectorData &_vectorData);
	uint8_t	ReadVectorRAM(uint16_t address);
	uint8_t	ReadVectorROM(uint16_t address);
	void		Reset(void);
	void		WriteVectorRAM(uint16_t address, uint8_t value);

private:
	VectorData	vectorData;
	bool haveNewData;
	uint16_t haltCycle;
};

#pragma warning(pop)

#endif	/* VECTORGENERATOR_H */

