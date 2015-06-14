
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
}

