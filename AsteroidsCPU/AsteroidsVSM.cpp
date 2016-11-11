
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
   a7_1._PRE().SetSource(nand1);

   // D flip flop #2 at A7
   a7_2.D().SetSource(clock.VGCK());
   a7_1.Clock().SetSource(clock.C6MHz());
   a7_1._CLR().SetSource(VCC);
   a7_1._PRE().SetSource(d8_1._Q());
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
   isHalt = false;
   dvy = 0;
   timer0 = false;
   timer1 = false;
   timer2 = false;
   timer3 = false;

   while (!isHalt)
   {
      clock.Tick();
   }
}
