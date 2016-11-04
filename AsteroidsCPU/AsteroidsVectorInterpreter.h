
#ifndef ASTEROIDSVECTORINTERPRETER_H
#define ASTEROIDSVECTORINTERPRETER_H

#include "TempestIO/Vector/SimpleVector.h"

class AsteroidsVectorInterpreter
{
public:
   void Interpret(void);
   void SetVectorRAM(const void *vectorRAM);
   void GetAllVectors(std::vector<SimpleVector> &vectors);

private:
   uint8_t vectorRAM[0x800];
};

#endif
