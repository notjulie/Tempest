
#ifndef AM2901TESTINTERFACE_H
#define AM2901TESTINTERFACE_H

#include "MathBox/Am2901.h"

class Am2901TestInterface
{
public:
	static Nybble GetRAMValue(Am2901 *alu, Nybble address);
};


#endif
