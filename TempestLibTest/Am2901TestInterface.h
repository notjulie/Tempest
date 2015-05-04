
#ifndef AM2901TESTINTERFACE_H
#define AM2901TESTINTERFACE_H

#include "MathBox/Am2901.h"
#include "MathBox/Nullable.h"

class Am2901TestInterface
{
public:
	static NullableNybble GetRAMValue(Am2901 *alu, NullableNybble address);
};


#endif
