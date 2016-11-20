
#ifndef ASTEROIDSVSM_H
#define ASTEROIDSVSM_H

#include "TempestIO/Vector/SimpleVector.h"

#include "ANDGate.h"
#include "AsteroidsVectorMemory.h"
#include "LogicBoard.h"
#include "LS109.h"
#include "LS174.h"
#include "LS42.h"
#include "LS74.h"
#include "NANDGate.h"
#include "ORGate.h"
#include "VSMClock.h"
#include "VSMROM.h"


class AsteroidsVSM {
public:
   AsteroidsVSM(void);

   void Interpret(void);
   void GetAllVectors(std::vector<SimpleVector> &vectors);
   void SetVectorRAM(const void *vectorRAM, int size);

   // low level public methods
   void Go(void);
   void Reset(void);
   bool IsHalted(void) const { return halt.Get(); }

private:
   std::vector<SimpleVector> vectors;
   LogicBoard board;

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
   LS174 dataLatch0;
   LS174 dataLatch1;
   LS174 dataLatch2;
   LS174 dataLatch3;
   LS42 vsmDecoder;
   LS109 goSource;
   LogicSignal _go;
   Inverter goStrobe;
   ORGate vsmROMLatchClockSource;
   NANDGate alphanumSource;
   ANDGate latch1ResetSource;
   ANDGate latch0ResetSource;
   ANDGate latch3ClockSource;
   ORGate latch3ClockEnable;
   NANDGate decoderHighBitSource;
   AsteroidsVectorMemory vectorMemory;

   // traces
   LogicSignal halt;
   LogicSignal _halt;
   LogicSignal scale0;
   LogicSignal scale1;
   LogicSignal scale2;
   LogicSignal scale3;
   LogicSignal timer0;
   LogicSignal timer1;
   LogicSignal timer2;
   LogicSignal timer3;
   LogicSignal dvx[12];
   LogicSignal dvy[12];
   LogicSignal _latch0;
   LogicSignal _latch1;
   LogicSignal _latch2;
   LogicSignal _latch3;
   LogicSignal _dmaPush;
   LogicSignal _dmaLoad;
   LogicSignal _goStrobe;
   LogicSignal _haltStrobe;
   LogicSignal ddma[8];
   LogicSignal _alphaNum;

   // things that may require some thought
   LogicSignal xCarry;
   LogicSignal yCarry;
   LogicSignal _dmaCut;
   LogicSignal _stop;
};


#endif
