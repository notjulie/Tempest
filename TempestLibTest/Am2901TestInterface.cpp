
#include "stdafx.h"
#include "Am2901TestInterface.h"


NullableNybble Am2901TestInterface::GetRAMValue(Am2901 *alu, NullableNybble address)
{
	return alu->RAM[address.Value().Value()];
}

