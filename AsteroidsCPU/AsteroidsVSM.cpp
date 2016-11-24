
#include "stdafx.h"
#include "AsteroidsVSM.h"


AsteroidsVSM::AsteroidsVSM(void)
{
   // set our constant values
   _VMEM.Set(true);
   VCC.Set(true);

   // wire all our components together

   // D flip flop #1 at A7
   board.Connect(_VMEM, a7_1.D());
   board.Connect(clock.VGCK(), a7_1.Clock());
   board.Connect(VCC, a7_1._CLR());
   board.Connect(nandA8_1, a7_1._PRE());

   // vgClockDelay D flip flop; this just generates a delayed version of the VGCK
   // signal
   board.Connect(clock.VGCK(), vgClockDelay.D());
   board.Connect(clock.C6MHz(), vgClockDelay.Clock());
   board.Connect(VCC, vgClockDelay._CLR());
   board.Connect(VCC, vgClockDelay._PRE());

   // D flip flop #1 at D8
   board.Connect(nandH5, d8_1.D());
   board.Connect(clock.VGCK(), d8_1.Clock());
   board.Connect(VCC, d8_1._CLR());
   board.Connect(VCC, d8_1._PRE());

   // NAND gate at A8
   nandA8_1.AddSource(vgClockDelay.Q());
   nandA8_1.AddSource(inverterL6);

   // NAND gate at H5
   nandH5.AddSource(xCarry);
   nandH5.AddSource(yCarry);

   // inverter at L6
   board.Connect(vsmROMLatch.GetOutput(4), inverterL6);

   // vsmROMLatch
   vsmROMLatch.SetName("vsmROMLatch");
   board.Connect(vsmROM.GetOutput(2), vsmROMLatch.GetInput(2));
   board.Connect(vsmROM.GetOutput(4), vsmROMLatch.GetInput(3));
   board.Connect(vsmROM.GetOutput(3), vsmROMLatch.GetInput(4));
   board.Connect(vsmROM.GetOutput(1), vsmROMLatch.GetInput(5));
   board.Connect(vsmROM.GetOutput(halt), vsmROMLatch.GetInput(6));
   board.Connect(vsmROMLatchClockSource, vsmROMLatch.Clock());
   board.Connect(_reset, vsmROMLatch._Clear());

   // vsmROM
   board.Connect(vsmROMLatch.GetOutput(5), vsmROM.GetInput(0));
   board.Connect(vsmROMLatch.GetOutput(2), vsmROM.GetInput(1));
   board.Connect(vsmROMLatch.GetOutput(4), vsmROM.GetInput(2));
   board.Connect(vsmROMLatch.GetOutput(3), vsmROM.GetInput(3));
   board.Connect(andT0T3, vsmROM.GetInput(4));
   board.Connect(andT1T3, vsmROM.GetInput(5));
   board.Connect(andT2T3, vsmROM.GetInput(6));
   board.Connect(andGoHalt, vsmROM.GetInput(7));

   // haltLatch
   haltLatch.SetName("haltLatch");
   board.Connect(timer0, haltLatch.J());
   board.Connect(timer0, haltLatch.K());
   board.Connect(_haltStrobe, haltLatch.Clock());
   board.Connect(_reset, haltLatch._PRE());
   board.Connect(andDMAGO, haltLatch._CLR());
   board.Connect(haltLatch.Q(), halt);
   board.Connect(haltLatch._Q(), _halt);

   // andDMAGO
   andDMAGO.AddSource(_dmaGo);
   andDMAGO.AddSource(_dmaCut);

   // timer AND gates
   andT0T3.SetName("andT0T3");
   andT0T3.AddSource(timer0);
   andT0T3.AddSource(timer3);
   andT1T3.SetName("andT1T3");
   andT1T3.AddSource(timer1);
   andT1T3.AddSource(timer3);
   andT2T3.SetName("andT2T3");
   andT2T3.AddSource(timer2);
   andT2T3.AddSource(timer3);

   // andGoHalt
   andGoHalt.SetName("andGoHalt");
   andGoHalt.AddSource(_go);
   andGoHalt.AddSource(_halt);

   // dataLatch0
   for (int i = 0; i < 8; ++i)
      board.Connect(ddma[i], dataLatch0.GetInput(1 + i));
   board.Connect(_latch0, dataLatch0.Clock());
   board.Connect(latch0ResetSource, dataLatch0._Clear());
   for (int i = 0; i < 8; ++i)
      board.Connect(dataLatch0.GetOutput(1 + i), dvy[i]);

   // dataLatch1
   dataLatch1.SetName("dataLatch1");
   for (int i = 0; i < 8; ++i)
      board.Connect(ddma[i], dataLatch1.GetInput(1 + i));
   board.Connect(_latch1, dataLatch1.Clock());
   board.Connect(latch1ResetSource, dataLatch1._Clear());
   board.Connect(dataLatch1.GetOutput(1), dvy[8]);
   board.Connect(dataLatch1.GetOutput(2), dvy[9]);
   board.Connect(dataLatch1.GetOutput(3), dvy[10]);
   board.Connect(dataLatch1.GetOutput(4), dvy[11]);
   board.Connect(dataLatch1.GetOutput(5), timer0);
   board.Connect(dataLatch1.GetOutput(6), timer1);
   board.Connect(dataLatch1.GetOutput(7), timer2);
   board.Connect(dataLatch1.GetOutput(8), timer3);

   // dataLatch2
   for (int i = 0; i < 8; ++i)
      board.Connect(ddma[i], dataLatch2.GetInput(1 + i));
   board.Connect(_latch2, dataLatch2.Clock());
   board.Connect(_alphaNum, dataLatch2._Clear());
   for (int i = 0; i < 8; ++i)
      board.Connect(dataLatch2.GetOutput(1 + i), dvx[i]);

   // dataLatch3
   for (int i = 0; i < 8; ++i)
      board.Connect(ddma[i], dataLatch3.GetInput(1 + i));
   board.Connect(latch3ClockSource, dataLatch3.Clock());
   board.Connect(VCC, dataLatch3._Clear());
   board.Connect(dataLatch3.GetOutput(1), dvx[8]);
   board.Connect(dataLatch3.GetOutput(2), dvx[9]);
   board.Connect(dataLatch3.GetOutput(3), dvx[10]);
   board.Connect(dataLatch3.GetOutput(4), dvx[11]);
   board.Connect(dataLatch3.GetOutput(5), scale0);
   board.Connect(dataLatch3.GetOutput(6), scale1);
   board.Connect(dataLatch3.GetOutput(7), scale2);
   board.Connect(dataLatch3.GetOutput(8), scale3);

   // vsmDecoder
   board.Connect(vsmROMLatch.GetOutput(5), vsmDecoder.A());
   board.Connect(vsmROMLatch.GetOutput(2), vsmDecoder.B());
   board.Connect(vsmROMLatch.GetOutput(4), vsmDecoder.C());
   board.Connect(decoderHighBitSource, vsmDecoder.D());
   board.Connect(vsmDecoder.GetOutput(0), _dmaPush);
   board.Connect(vsmDecoder.GetOutput(1), _dmaLoad);
   board.Connect(vsmDecoder.GetOutput(2), _goStrobe);
   board.Connect(vsmDecoder.GetOutput(3), _haltStrobe);
   board.Connect(vsmDecoder.GetOutput(4), _latch0);
   board.Connect(vsmDecoder.GetOutput(5), _latch1);
   board.Connect(vsmDecoder.GetOutput(6), _latch2);
   board.Connect(vsmDecoder.GetOutput(7), _latch3);

   // goSource
   board.Connect(goStrobe, goSource.J());
   board.Connect(_stop, goSource.K());
   board.Connect(clock.C6MHz(), goSource.Clock());
   board.Connect(VCC, goSource._PRE());
   board.Connect(_halt, goSource._CLR());
   board.Connect(goSource._Q(), _go);

   // goStrobe
   board.Connect(_goStrobe, goStrobe);

   //vsmROMLatchClockSource
   vsmROMLatchClockSource.SetName("vsmROMLatchClockSource");
   vsmROMLatchClockSource.AddSource(clock.C6MHz());
   vsmROMLatchClockSource.AddSource(a7_1._Q());

   // ddma
   for (int i = 0; i < 8; ++i)
      board.Connect(vectorMemory.GetOutput(i), ddma[i]);

   // vectorMemory
   board.Connect(pcCounterClock, vectorMemory.CounterClock());
   board.Connect(_dmaLoad, vectorMemory._CounterLoad());
   board.Connect(_timer0, vectorMemory._StackRead());

   // _timer0
   board.Connect(timer0, _timer0);

   // pcCounterClock
   pcCounterClock.AddSource(_latch1);
   pcCounterClock.AddSource(_latch3);

   // alphanum
   alphanumSource.AddSource(timer0);
   alphanumSource.AddSource(timer1);
   alphanumSource.AddSource(timer2);
   alphanumSource.AddSource(timer3);
   board.Connect(alphanumSource, _alphaNum);

   // latch1ResetSource
   latch1ResetSource.AddSource(_reset);
   latch1ResetSource.AddSource(_dmaGo);

   // latch0ResetSource
   latch0ResetSource.AddSource(_alphaNum);
   latch0ResetSource.AddSource(latch1ResetSource);

   // latch3ClockSource
   latch3ClockSource.AddSource(_latch3);
   latch3ClockSource.AddSource(latch3ClockEnable);

   // latch3ClockEnable
   latch3ClockEnable.AddSource(_latch0);
   latch3ClockEnable.AddSource(_alphaNum);

   // decoderHighBitSource
   decoderHighBitSource.AddSource(vsmROMLatch.GetOutput(3));
   decoderHighBitSource.AddSource(a7_1.Q());
   decoderHighBitSource.AddSource(vgClockDelay.Q());

   // adma0
   board.Connect(vsmROMLatch.GetOutput(5), adma0);
   board.Connect(adma0, vectorMemory.AddressLowBit());

   // name our signals
   _haltStrobe.SetName("_haltStrobe");
   timer0.SetName("timer0");
   timer1.SetName("timer1");
   timer2.SetName("timer2");
   timer3.SetName("timer3");

   // propogate any activity that might have happened when we were connecting
   // parts together
   board.PropogateSignals();
}

void AsteroidsVSM::GetAllVectors(std::vector<SimpleVector> &vectors)
{
   vectors = this->vectors;
}

void AsteroidsVSM::SetVectorRAM(const void *vectorRAM, int size)
{
   vectorMemory.SetVectorRAM(vectorRAM, size);
}

void AsteroidsVSM::Interpret(void)
{
   // reset
   Reset();

   // assert the GO line
   Go();

   // loop until halted
   while (!halt)
   {
      clock.Tick();
      board.PropogateSignals();
   }
}

void AsteroidsVSM::Go(void)
{
   // assert _dmaGo and propogate changes
   _dmaGo.Set(false);
   board.PropogateSignals();

   // de-assert _dmaGo and propogate changes
   _dmaGo.Set(true);
   board.PropogateSignals();
}

void AsteroidsVSM::Reset(void)
{
   // these currently aren't set by anything so set them here
   _dmaCut.Set(true);
   _stop.Set(true);

   // we shouldn't be getting DMAGO during reset
   _dmaGo.Set(true);
   board.PropogateSignals();

   // pull the reset line
   _reset.Set(false);
   board.PropogateSignals();

   // run the clock a little
   for (int i = 0; i < 10; ++i)
   {
      clock.Tick();
      board.PropogateSignals();
   }

   // unpull the reset line
   _reset.Set(true);
   board.PropogateSignals();
}

