
#ifndef DISCOVECTOR_H
#define DISCOVECTOR_H

#include "DiscoDAC.h"

class DiscoVector
{
public:
	DiscoVector(void);
	void Init(void);

private:
	DiscoDAC dac1;
	DiscoDAC dac2;
};

extern DiscoVector Vector;

#endif
