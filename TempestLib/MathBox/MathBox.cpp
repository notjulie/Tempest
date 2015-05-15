// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Emulation of the MathBox; this emulates all the logic external
//    to the 4 Am2901 chips... those are emulated in class Am2901.
// ====================================================================

#include "MathBoxAFX.h"

#include "AbstractTempestEnvironment.h"

#include "MathBox.h"

class Timer
{
public:
	Timer(AbstractTempestEnvironment	*_environment, uint32_t *_value) {
		environment = _environment;
		value = _value;
		startTime = _environment->GetMicrosecondCount();
	}
	~Timer(void) {
		*value = (uint32_t)(*value + (environment->GetMicrosecondCount() - startTime));
	}

private:
	AbstractTempestEnvironment	*environment;
	uint32_t *value;
	uint32_t startTime;
};

MathBox::MathBox(AbstractTempestEnvironment	*_environment)
{
	// save parameters
	environment = _environment;

	// clear
	PC = 0;
	BEGIN = false;
	Q0Latch = false;
	dataIn = 0;
	addressIn = 0;
	STOP = false;

	// clear profiling info
	totalMathBoxTime = 0;
	totalMathBoxWrites = 0;
	totalRisingClockTime = 0;
	totalFallingClockTime = 0;
	totalSetALUInputsTime = 0;
	totalSetALUCarryTime = 0;
	totalALUFallingClockTime = 0;
}

MathBox::~MathBox(void)
{
}

void MathBox::LoadROM(const uint8_t *rom, int length, char slot)
{
	switch (slot)
	{
	case 'A':
		if (length != sizeof(romA))
			return;
		memcpy(&romA[0], rom, (unsigned)length);
		break;

	case 'E':
		if (length != sizeof(romE))
			return;
		memcpy(&romE[0], rom, (unsigned)length);
		break;

	case 'F':
		if (length != sizeof(romF))
			return;
		memcpy(&romF[0], rom, (unsigned)length);
		break;

	case 'H':
		if (length != sizeof(romH))
			return;
		memcpy(&romH[0], rom, (unsigned)length);
		break;

	case 'J':
		if (length != sizeof(romJ))
			return;
		memcpy(&romJ[0], rom, (unsigned)length);
		break;

	case 'K':
		if (length != sizeof(romK))
			return;
		memcpy(&romK[0], rom, (unsigned)length);
		break;

	case 'L':
		if (length != sizeof(romL))
			return;
		memcpy(&romL[0], rom, (unsigned)length);
		break;
	}
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
	++totalMathBoxWrites;
	uint32_t usStart = environment->GetMicrosecondCount();

	// set our inputs... the address strobe will assert BEGIN and cause the
	// first rising edge of the clock
	addressIn = address;
	dataIn = value;
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
	dataIn = 0;

	// then we can just handle clock pulses until the clock is disabled
	while (!STOP)
	{
		HandleRisingClock();
		HandleFallingClock();
	}

	uint32_t usEnd = environment->GetMicrosecondCount();
	totalMathBoxTime = totalMathBoxTime + (usEnd - usStart);

	if (totalMathBoxWrites == 10000)
		totalMathBoxWrites = 10000;
}


void MathBox::HandleRisingClock(void)
{
	uint32_t usStart = environment->GetMicrosecondCount();

	// calculate the new PC 
	uint8_t newPC;
	bool pcen;
	{
		bool E5XORout = aluE.GetOVR() ^ aluE.GetF3();
		bool D4NAND1out = !(E5XORout && ((romE[PC] & 8) != 0));
		bool D4NAND2out = !(D4NAND1out && ((romE[PC] & 4) != 0));
		pcen = BEGIN || !D4NAND2out;
	}
	if (pcen)
	{
		// we load the PC from whichever source is selected
		if (BEGIN)
			newPC = romA[(unsigned)addressIn];
		else
			newPC = JumpLatch;
	}
	else
	{
		newPC = (uint8_t)(PC + 1);
	}

	// calculate the new value of Q0Latch
	bool newQ0Latch = GetQ0();

	// let the ALUs handle the rising clock edge...
	SetALUInputs();
	aluK.SetClock(true);
	aluF.SetClock(true);
	aluJ.SetClock(true);
	aluE.SetClock(true);

	// latch all the state values that we are supposed to latch on the
	// rising clock
	PC = newPC;
	Q0Latch = newQ0Latch;

	uint32_t usEnd = environment->GetMicrosecondCount();
	totalRisingClockTime = totalRisingClockTime + (usEnd - usStart);
}

void MathBox::HandleFallingClock(void)
{
	uint32_t usStart = environment->GetMicrosecondCount();

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
	{
		Timer timer(environment, &this->totalALUFallingClockTime);
		aluK.SetClock(false);
		aluF.SetClock(false);
		aluJ.SetClock(false);
		aluE.SetClock(false);
	}

	// latch all the state values that we are supposed to latch on the
	// falling clock
	STOP = newSTOP;
	JumpLatch = newJumpLatch;

	uint32_t usEnd = environment->GetMicrosecondCount();
	totalFallingClockTime = totalFallingClockTime + (usEnd - usStart);
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
	Timer timer(environment, &totalSetALUInputsTime);

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

	// set the data inputs accordingly
	aluK.DataIn = aluJ.DataIn = (uint8_t)(dataIn & 0xF);
	aluF.DataIn = aluE.DataIn = (uint8_t)(dataIn >> 4);
}


void MathBox::SetALUCarryFlags(void)
{
	Timer timer(environment, &totalSetALUCarryTime);

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

