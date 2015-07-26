
#ifndef DISCOVECTOR_H
#define DISCOVECTOR_H

#include "TempestIO/Vector/SimpleVector.h"
#include "Drivers/DualDAC.h"


class DiscoVector
{
public:
	DiscoVector(void);
	void Init(void);
	void Service(void);

private:
	int CreateDualRamp(uint16_t fromX, uint16_t toX, uint16_t fromY, uint16_t toY);

private:
	DualDAC dac;
	SimpleVector *currentVectors;
	int		currentVectorCount;
	SimpleVector	defaultVectorList[4];
	uint32_t dacBuffer[4096];
};

extern DiscoVector Vector;

#endif
