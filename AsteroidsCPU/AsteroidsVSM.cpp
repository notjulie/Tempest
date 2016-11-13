
#include "stdafx.h"
#include "AsteroidsVSM.h"


AsteroidsVSM::AsteroidsVSM(void)
{
   // set our constant values
   _VMEM.Set(true);
   VCC.Set(true);

   // wire all our components together

   // D flip flop #1 at A7
   a7_1.D().SetSource(_VMEM);
   a7_1.Clock().SetSource(clock.VGCK());
   a7_1._CLR().SetSource(VCC);
   a7_1._PRE().SetSource(nandA8_1);

   // D flip flop #2 at A7
   a7_2.D().SetSource(clock.VGCK());
   a7_2.Clock().SetSource(clock.C6MHz());
   a7_2._CLR().SetSource(VCC);
   a7_2._PRE().SetSource(d8_1._Q());

   // D flip flop #1 at D8
   d8_1.D().SetSource(nandH5);
   d8_1.Clock().SetSource(clock.VGCK());
   d8_1._CLR().SetSource(VCC);
   d8_1._PRE().SetSource(VCC);

   // NAND gate at A8
   nandA8_1.AddSource(a7_2.Q());
   nandA8_1.AddSource(inverterL6);

   // NAND gate at H5
   nandH5.AddSource(xCarry);
   nandH5.AddSource(yCarry);

   // inverter at L6
   inverterL6.SetSource(vsmROMLatch.GetOutput(4));

   // vsmROMLatch
   vsmROMLatch.GetInput(2).SetSource(vsmROM.GetOutput(2));
   vsmROMLatch.GetInput(3).SetSource(vsmROM.GetOutput(4));
   vsmROMLatch.GetInput(4).SetSource(vsmROM.GetOutput(3));
   vsmROMLatch.GetInput(5).SetSource(vsmROM.GetOutput(1));
   vsmROMLatch.GetInput(6).SetSource(vsmROM.GetOutput(halt));
   vsmROMLatch.Clock().SetSource(vsmROMLatchClockSource);
   vsmROMLatch._Clear().SetSource(_reset);

   // vsmROM
   vsmROM.GetInput(0).SetSource(vsmROMLatch.GetOutput(5));
   vsmROM.GetInput(1).SetSource(vsmROMLatch.GetOutput(2));
   vsmROM.GetInput(2).SetSource(vsmROMLatch.GetOutput(4));
   vsmROM.GetInput(3).SetSource(vsmROMLatch.GetOutput(3));
   vsmROM.GetInput(4).SetSource(andT0T3);
   vsmROM.GetInput(5).SetSource(andT1T3);
   vsmROM.GetInput(6).SetSource(andT2T3);
   vsmROM.GetInput(7).SetSource(andGoHalt);

   // haltLatch
   haltLatch.J().SetSource(timer0);
   haltLatch.K().SetSource(timer0);
   haltLatch.Clock().SetSource(_haltStrobe);
   haltLatch._PRE().SetSource(_reset);
   haltLatch._CLR().SetSource(andDMAGO);
   halt.SetSource(haltLatch.Q());
   _halt.SetSource(haltLatch._Q());

   // andDMAGO
   andDMAGO.AddSource(_dmaGo);
   andDMAGO.AddSource(_dmaCut);

   // timer AND gates
   andT0T3.AddSource(timer0);
   andT0T3.AddSource(timer3);
   andT1T3.AddSource(timer1);
   andT1T3.AddSource(timer3);
   andT2T3.AddSource(timer2);
   andT2T3.AddSource(timer3);

   // andGoHalt
   andGoHalt.AddSource(_go);
   andGoHalt.AddSource(_halt);

   // dataLatch0
   for (int i = 0; i < 8; ++i)
      dataLatch0.GetInput(1 + i).SetSource(ddma[i]);
   dataLatch0.Clock().SetSource(_latch0);
   dataLatch0._Clear().SetSource(latch0ResetSource);
   for (int i = 0; i < 8; ++i)
      dvy[i].SetSource(dataLatch0.GetOutput(1+i));

   // dataLatch1
   for (int i = 0; i < 8; ++i)
      dataLatch1.GetInput(1+i).SetSource(ddma[i]);
   dataLatch1.Clock().SetSource(_latch1);
   dataLatch1._Clear().SetSource(latch1ResetSource);
   dvy[8].SetSource(dataLatch1.GetOutput(1));
   dvy[9].SetSource(dataLatch1.GetOutput(2));
   dvy[10].SetSource(dataLatch1.GetOutput(3));
   dvy[11].SetSource(dataLatch1.GetOutput(4));
   timer0.SetSource(dataLatch1.GetOutput(5));
   timer1.SetSource(dataLatch1.GetOutput(6));
   timer2.SetSource(dataLatch1.GetOutput(7));
   timer3.SetSource(dataLatch1.GetOutput(8));

   // dataLatch2
   for (int i = 0; i < 8; ++i)
      dataLatch2.GetInput(1 + i).SetSource(ddma[i]);
   dataLatch2.Clock().SetSource(_latch2);
   dataLatch2._Clear().SetSource(_alphaNum);
   for (int i = 0; i < 8; ++i)
      dvx[i].SetSource(dataLatch2.GetOutput(1 + i));

   // dataLatch3
   for (int i = 0; i < 8; ++i)
      dataLatch3.GetInput(1 + i).SetSource(ddma[i]);
   dataLatch3.Clock().SetSource(latch3ClockSource);
   dataLatch3._Clear().SetSource(VCC);
   dvx[8].SetSource(dataLatch3.GetOutput(1));
   dvx[9].SetSource(dataLatch3.GetOutput(2));
   dvx[10].SetSource(dataLatch3.GetOutput(3));
   dvx[11].SetSource(dataLatch3.GetOutput(4));
   scale0.SetSource(dataLatch3.GetOutput(5));
   scale1.SetSource(dataLatch3.GetOutput(6));
   scale2.SetSource(dataLatch3.GetOutput(7));
   scale3.SetSource(dataLatch3.GetOutput(8));

   // vsmDecoder
   vsmDecoder.A().SetSource(vsmROMLatch.GetOutput(5));
   vsmDecoder.B().SetSource(vsmROMLatch.GetOutput(2));
   vsmDecoder.C().SetSource(vsmROMLatch.GetOutput(4));
   vsmDecoder.D().SetSource(decoderHighBitSource);
   _dmaPush.SetSource(vsmDecoder.GetOutput(0));
   _dmaLoad.SetSource(vsmDecoder.GetOutput(1));
   _goStrobe.SetSource(vsmDecoder.GetOutput(2));
   _haltStrobe.SetSource(vsmDecoder.GetOutput(3));
   _latch0.SetSource(vsmDecoder.GetOutput(4));
   _latch1.SetSource(vsmDecoder.GetOutput(5));
   _latch2.SetSource(vsmDecoder.GetOutput(6));
   _latch3.SetSource(vsmDecoder.GetOutput(7));

   // goSource
   goSource.J().SetSource(goStrobe);
   goSource.K().SetSource(_stop);
   goSource.Clock().SetSource(clock.C6MHz());
   goSource._PRE().SetSource(VCC);
   goSource._CLR().SetSource(_halt);
   _go.SetSource(goSource._Q());

   // goStrobe
   goStrobe.SetSource(_goStrobe);

   //vsmROMLatchClockSource
   vsmROMLatchClockSource.AddSource(clock.C6MHz());
   vsmROMLatchClockSource.AddSource(a7_1._Q());

   // ddma
   for (int i = 0; i < 8; ++i)
      ddma[0].SetSource(vectorMemory.GetOutput(i));

   // alphanum
   alphanumSource.AddSource(timer0);
   alphanumSource.AddSource(timer1);
   alphanumSource.AddSource(timer2);
   alphanumSource.AddSource(timer3);
   _alphaNum.SetSource(alphanumSource);

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
   decoderHighBitSource.AddSource(a7_2.Q());
}

void AsteroidsVSM::GetAllVectors(std::vector<SimpleVector> &vectors)
{
   vectors = this->vectors;
}

void AsteroidsVSM::SetVectorRAM(const void *vectorRAM)
{
   vectorMemory.SetVectorRAM(vectorRAM);
}

void AsteroidsVSM::Interpret(void)
{
   // reset
   _reset.Set(false);
   for (int i = 0; i < 10; ++i)
      clock.Tick();
   _reset.Set(true);

   // assert the GO line
   _dmaGo.Set(false);

   while (!halt)
   {
      clock.Tick();
   }
}
