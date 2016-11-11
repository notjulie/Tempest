
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

   // vsmROM
   vsmROM.GetInput(0).SetSource(vsmROMLatch.GetOutput(5));
   vsmROM.GetInput(1).SetSource(vsmROMLatch.GetOutput(2));
   vsmROM.GetInput(2).SetSource(vsmROMLatch.GetOutput(4));
   vsmROM.GetInput(3).SetSource(vsmROMLatch.GetOutput(3));
   vsmROM.GetInput(4).SetSource(andT0T3);
   vsmROM.GetInput(5).SetSource(andT1T3);
   vsmROM.GetInput(6).SetSource(andT2T3);
   vsmROM.GetInput(7).SetSource(andGoHalt);
}

void AsteroidsVSM::GetAllVectors(std::vector<SimpleVector> &vectors)
{
   vectors = this->vectors;
}

void AsteroidsVSM::SetVectorRAM(const void *vectorRAM)
{
   memcpy(this->vectorRAM, vectorRAM, 0x800);
}

void AsteroidsVSM::Interpret(void)
{
   // clear
   dvy = 0;
   timer0 = false;
   timer1 = false;
   timer2 = false;
   timer3 = false;

   while (!halt)
   {
      clock.Tick();
   }
}
