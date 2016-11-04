
#include "stdafx.h"
#include "AsteroidsVectorInterpreter.h"


void AsteroidsVectorInterpreter::SetVectorRAM(const void *vectorRAM)
{
   memcpy(this->vectorRAM, vectorRAM, 0x800);
}

void AsteroidsVectorInterpreter::Interpret(void)
{
}

void AsteroidsVectorInterpreter::GetAllVectors(std::vector<SimpleVector> &vectors)
{
   vectors.resize(0);
}
