
#ifndef ASTEROIDSVSM_H
#define ASTEROIDSVSM_H

#include "TempestIO/Vector/SimpleVector.h"

#include "ANDGate.h"
#include "LS109.h"
#include "LS174.h"
#include "LS74.h"
#include "NANDGate.h"
#include "VSMClock.h"
#include "VSMROM.h"


class AsteroidsVSM {
public:
   AsteroidsVSM(void);

   void Interpret(void);
   void GetAllVectors(std::vector<SimpleVector> &vectors);
   void SetVectorRAM(const void *vectorRAM);

private:
   uint8_t vectorRAM[0x800];
   std::vector<SimpleVector> vectors;

   // manually controlled signals
   LogicSignal _VMEM;
   LogicSignal VCC;
   LogicSignal _reset;
   LogicSignal _dmaGo;

   // logic, already wired
   VSMClock clock;
   LS74 a7_1;
   LS74 a7_2;
   LS74 d8_1;
   NANDGate nandA8_1;
   NANDGate nandH5;
   Inverter inverterL6;
   LS174 vsmROMLatch;
   VSMROM vsmROM;
   LS109 haltLatch;
   ANDGate andDMAGO;
   ANDGate andT0T3;
   ANDGate andT1T3;
   ANDGate andT2T3;
   ANDGate andGoHalt;

   // traces
   LogicSignal halt;
   LogicSignal _halt;

   // logic, not wired yet
   LogicSignal timer0;
   LogicSignal timer1;
   LogicSignal timer2;
   LogicSignal timer3;
   LogicSignal _haltStrobe;
   LogicSignal _dmaCut;
   LogicSignal _go;

   // things that may require some thought
   LogicSignal xCarry;
   LogicSignal yCarry;

   uint16_t dvy = 0;
};


#endif
