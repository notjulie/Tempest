
#include "TempestDisco.h"
#include "DiscoVector.h"

DiscoVector Vector;

DiscoVector::DiscoVector(void)
{
}

void DiscoVector::Init(void)
{
	dac1.Init(1);
	dac2.Init(2);

	dac1.StartRamp(0, 4095, 10000000);
	dac2.StartRamp(0, 4095, 10000000);
}

