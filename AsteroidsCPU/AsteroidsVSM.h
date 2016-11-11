
#ifndef ASTEROIDSVSM_H
#define ASTEROIDSVSM_H

#include "TempestIO/Vector/SimpleVector.h"

#include "LS174.h"
#include "LS74.h"
#include "NANDGate.h"
#include "VSMClock.h"


class AsteroidsVSM {
public:
   AsteroidsVSM(void);

   void Interpret(void);
   void GetAllVectors(std::vector<SimpleVector> &vectors);
   void SetVectorRAM(const void *vectorRAM);

private:
   uint8_t vectorRAM[0x800];
   std::vector<SimpleVector> vectors;

   // constants
   LogicSignal _VMEM;
   LogicSignal VCC;

   // logic, already wired
   VSMClock clock;
   LS74 a7_1;
   LS74 a7_2;
   LS74 d8_1;
   NANDGate nandA8_1;
   NANDGate nandH5;
   Inverter inverterL6;

   // logic, not wired yet
   LogicSignal xCarry;
   LogicSignal yCarry;
   LS174 vsmROMLatch;

   bool isHalt = true;
   uint16_t dvy = 0;
   bool timer0 = false;
   bool timer1 = false;
   bool timer2 = false;
   bool timer3 = false;
};


#endif
