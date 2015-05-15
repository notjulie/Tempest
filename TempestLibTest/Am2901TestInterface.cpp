
#include "stdafx.h"
#include "Am2901TestInterface.h"


Nybble Am2901TestInterface::GetRAMValue(Am2901 *alu, Nybble address)
{
	return alu->RAM[address.Value()];
}

