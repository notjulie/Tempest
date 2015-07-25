
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
	DiscoDAC dac1;
	DiscoDAC dac2;
	SimpleVector *currentVectors;
	int		currentVectorCount;
	SimpleVector	defaultVectorList[4];
};

extern DiscoVector Vector;

#endif
