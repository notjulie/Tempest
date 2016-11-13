
#include "stdafx.h"
#include "AsteroidsVectorMemory.h"


void AsteroidsVectorMemory::SetVectorRAM(const void *vectorRAM)
{
   memcpy(this->vectorRAM, vectorRAM, 0x800);
}

