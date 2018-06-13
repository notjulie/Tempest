// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Emulation of the MathBox; this emulates all the logic external
//    to the 4 Am2901 chips... those are emulated in class Am2901.
//    This is literally a hardware emulator... it simulates the step
//    by step operation of the signals in the math box circuit.
// ====================================================================

#include "MathBoxAFX.h"

#include "TempestROMS.h"

#include "MathBox.h"

MathBox::MathBox(void)
{
	// point to our ROMs
	romA = ROM_136002_126;
	romE = ROM_136002_127;
	romF = ROM_136002_128;
	romH = ROM_136002_129;
	romJ = ROM_136002_130;
	romK = ROM_136002_131;
	romL = ROM_136002_132;
}

MathBox::~MathBox(void)
{
}

uint8_t MathBox::GetStatus(void)
{
	// it appears that we are supposed to return 0x80 to indicate that
	// the mathbox is busy; this is never true because the emulated mathbox
	// does all its work synchronously
	return 0;
}

uint8_t MathBox::ReadLow(void)
{
	Nybble yK = aluK.GetY();
	Nybble yF = aluF.GetY();
	return (uint8_t)((yF.Value() << 4) +yK.Value());
}

uint8_t MathBox::ReadHigh(void)
{
	Nybble yJ = aluJ.GetY();
	Nybble yE = aluE.GetY();
	return (uint8_t)((yE.Value() << 4) + yJ.Value());
}

void MathBox::Write(uint8_t address, uint8_t value)
{
	// set our inputs... the address strobe will assert BEGIN and cause the
	// first rising edge of the clock
	addressIn = address;
   aluK.DataIn = aluJ.DataIn = (uint8_t)(value & 0xF);
   aluF.DataIn = aluE.DataIn = (uint8_t)(value >> 4);
	BEGIN = true;
	HandleRisingClock();

	// shortly after the clock will fall
	HandleFallingClock();

	// BEGIN clears, but since our clock runs at twice the speed of the main
	// CPU clock, the data and address lines will still be active for the beginning
	// of the next cycle
	BEGIN = false;
	if (STOP)
		return;

	// do the second cycle
	HandleRisingClock();
	HandleFallingClock();

	// now our inputs will clear
	addressIn = 0;
   aluK.DataIn = aluJ.DataIn = aluF.DataIn = aluE.DataIn = 0;

	// then we can just handle clock pulses until the clock is disabled
	while (!STOP)
	{
		HandleRisingClock();
		HandleFallingClock();
	}
}


void MathBox::HandleRisingClock(void)
{
	// calculate the new PC... these are signals that come from a handful
   // of logic gates, and I unrolled them this way to avoid having to check
   // the aluE outputs unless all the other conditions require it
	uint8_t newPC;
   if (BEGIN)
   {
      newPC = romA[(unsigned)addressIn];
   }
   else if (
      ((romE[PC] & 4) != 0) &&
      ((romE[PC] & 8) == 0 || !(aluE.GetOVR() ^ aluE.GetF3()))
      )
   {
      newPC = JumpLatch;
   }
	else
	{
		newPC = (uint8_t)(PC + 1);
	}

	// calculate the new value of Q0Latch
	bool newQ0Latch = GetQ0();

	// let the ALUs handle the rising clock edge...
	aluK.SetClock(true);
	aluF.SetClock(true);
	aluJ.SetClock(true);
	aluE.SetClock(true);

	// latch all the state values that we are supposed to latch on the
	// rising clock
	PC = newPC;
	Q0Latch = newQ0Latch;
}

void MathBox::HandleFallingClock(void)
{
	// calculate the new value of STOP
	bool newSTOP;
	if (BEGIN)
		newSTOP = false;
	else
		newSTOP = ((romH[PC] & 8) != 0);

	// new value of our jump address latch
	uint8_t newJumpLatch = JumpLatch;
	bool ldab = ((romF[PC] & 8) != 0);
	if (ldab)
		newJumpLatch = (uint8_t)((romL[PC]<<4) + romK[PC]);

	// let the ALUs handle the falling clock edge...
	SetALUInputs();

	// When the ALU clock is high is when its gates are active... now is the time to
	// set everybody's carry flags...
	SetALUCarryFlags();

	// dropping the clock latches the result
	aluK.SetClock(false);
	aluF.SetClock(false);
	aluJ.SetClock(false);
	aluE.SetClock(false);

	// latch all the state values that we are supposed to latch on the
	// falling clock
	STOP = newSTOP;
	JumpLatch = newJumpLatch;
}

bool MathBox::GetQ0(void)
{
	// =========== ASSUMPTION ============
	// If the ALUs are currently in a downshifting operation then we have Q0
	// being driven by A18 and the Q0 output of aluK.  I have assumed that they are
	// wired ORed in that case.
	switch (aluK.I678)
	{
	case 4:
	case 5:
		// downshifting operation
		return aluK.GetQ0Out() || !((romF[PC] & 2) != 0);

	default:
		// not downshifting, so aluK's Q0 is floating
		return !((romF[PC] & 2) != 0);
	}
}


void MathBox::SetALUInputs(void)
{
	// the A & B inputs to the ALUs are all the same
	aluK.AAddress = aluF.AAddress = aluJ.AAddress = aluE.AAddress = romL[PC];
	aluK.BAddress = aluF.BAddress = aluJ.BAddress = aluE.BAddress = romK[PC];

	// so are these
	aluK.I345 = aluF.I345 = aluJ.I345 = aluE.I345 = (uint8_t)(romH[PC] & 7);
	aluK.I678 = aluF.I678 = aluJ.I678 = aluE.I678 = (uint8_t)(romF[PC] & 7);

	// I012 are a little more complicated
	int i01 = romJ[PC] & 1;
	if (((romJ[PC] & 2) != 0) ^ (((romE[PC] & 2) != 0) && Q0Latch))
		i01 += 2;
	aluK.I012 = aluF.I012 = (uint8_t)(i01 + (romJ[PC] & 4));
	aluJ.I012 = aluE.I012 = (uint8_t)(i01 + ((romJ[PC] & 8) >> 1));
}


void MathBox::SetALUCarryFlags(void)
{
	// the actual carry flags are easy... they just cascade up
	aluK.CarryIn = ((romE[PC] & 1) != 0);
	aluF.CarryIn = aluK.GetCarryOut();
	aluJ.CarryIn = aluF.GetCarryOut();
	aluE.CarryIn = aluJ.GetCarryOut();

	// for our bit shifting flags between ALUs, we find what direction we're
	// shifting from I678, which we set the same for all ALUs, so just pick one
	switch (aluK.I678)
	{
	case 4:
	case 5:
		// downshifting operation
		{
			aluE.Q3In = aluK.GetRAM0Out();
			aluJ.Q3In = aluE.GetQ0Out();
			aluF.Q3In = aluJ.GetQ0Out();
			aluK.Q3In = aluF.GetQ0Out();

			bool r15;
			{
				bool E5XORout = aluE.GetOVR() ^ aluE.GetF3();
				bool D4NAND1out = !(E5XORout && ((romE[PC] & 8) != 0));
				r15 = !(D4NAND1out && !((romF[PC] & 2) != 0));
			}
			aluE.RAM3In = r15;
			aluJ.RAM3In = aluE.GetRAM0Out();
			aluF.RAM3In = aluJ.GetRAM0Out();
			aluK.RAM3In = aluF.GetRAM0Out();
		}
		break;

	case 6:
	case 7:
		// upshifting operation
		aluK.Q0In = (romF[PC] & 2) != 0;
		aluF.Q0In = aluK.GetQ3Out();
		aluJ.Q0In = aluF.GetQ3Out();
		aluE.Q0In = aluJ.GetQ3Out();

		aluK.RAM0In = aluE.GetQ3Out();
		aluF.RAM0In = aluK.GetRAM3Out();
		aluJ.RAM0In = aluF.GetRAM3Out();
		aluE.RAM0In = aluJ.GetRAM3Out();
		break;

	default:
		// not doing any shifting at all
		break;
	}
}

