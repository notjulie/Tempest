
#ifndef DISCOVECTOR_H
#define DISCOVECTOR_H

#include "TempestIO/Vector/SimpleVector.h"
#include "DiscoDAC.h"


class DiscoVector
{
public:
	DiscoVector(void);
	void Init(void);
	void Service(void);

private:
	int CreateRamp(uint16_t *rampBuffer, uint16_t from, uint16_t to);

private:
	DiscoDAC dac1;
	DiscoDAC dac2;
	SimpleVector *currentVectors;
	int		currentVectorCount;
	SimpleVector	defaultVectorList[4];
	uint16_t xDacBuffer[4096];
	uint16_t yDacBuffer[4096];
};

extern DiscoVector Vector;

#endif
