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


#include "stdafx.h"

#include "AbstractTempestEnvironment.h"
#include "MathBoxException.h"

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
			throw MathBoxException("Invalid size for ROM A");
		memcpy(&romA[0], rom, (unsigned)length);
		break;

	case 'E':
		if (length != sizeof(romE))
			throw MathBoxException("Invalid size for ROM E");
		memcpy(&romE[0], rom, (unsigned)length);
		break;

	case 'F':
		if (length != sizeof(romF))
			throw MathBoxException("Invalid size for ROM F");
		memcpy(&romF[0], rom, (unsigned)length);
		break;

	case 'H':
		if (length != sizeof(romH))
			throw MathBoxException("Invalid size for ROM H");
		memcpy(&romH[0], rom, (unsigned)length);
		break;

	case 'J':
		if (length != sizeof(romJ))
			throw MathBoxException("Invalid size for ROM J");
		memcpy(&romJ[0], rom, (unsigned)length);
		break;

	case 'K':
		if (length != sizeof(romK))
			throw MathBoxException("Invalid size for ROM K");
		memcpy(&romK[0], rom, (unsigned)length);
		break;

	case 'L':
		if (length != sizeof(romL))
			throw MathBoxException("Invalid size for ROM L");
		memcpy(&romL[0], rom, (unsigned)length);
		break;

	default:
		throw MathBoxException("MathBox::LoadROM: invalid slot letter");
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
	NullableNybble yK = aluK.GetY();
	NullableNybble yF = aluF.GetY();
	if (yK.IsUnknown() || yF.IsUnknown())
		throw MathBoxException("MathBox::ReadLow: value unknown");

	return (uint8_t)((yF.Value().Value() << 4) +yK.Value().Value());
}

uint8_t MathBox::ReadHigh(void)
{
	NullableNybble yJ = aluJ.GetY();
	NullableNybble yE = aluE.GetY();
	if (yJ.IsUnknown() || yE.IsUnknown())
		throw MathBoxException("MathBox::ReadHigh: value unknown");

	return (uint8_t)((yE.Value().Value() << 4) + yJ.Value().Value());
}

void MathBox::Write(uint8_t address, uint8_t value)
{
	++totalMathBoxWrites;
	uint32_t usStart = environment->GetMicrosecondCount();

	try
	{
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
		if (STOP.Value())
			return;

		// do the second cycle
		HandleRisingClock();
		HandleFallingClock();

		// now our inputs will clear
		addressIn = NullableByte::Unknown;
		dataIn = NullableByte::Unknown;

		// then we can just handle clock pulses until the clock is disabled
		while (!STOP.Value())
		{
			HandleRisingClock();
			HandleFallingClock();
		}
	}
	catch (MathBoxException &x)
	{
		SetError(x.what());
	}
	catch (...)
	{
		SetError("Unknown exception in MathBox::Write");
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
		{
			if (addressIn.IsUnknown())
				throw MathBoxException("Load PC from ROM A: addressIn not set");
			newPC = romA[(unsigned)addressIn.Value()];
		}
		else
		{
			newPC = JumpLatch;
		}
	}
	else
	{
		newPC = (uint8_t)(PC + 1);
		if (newPC == 0)
			throw MathBoxException("PC wraparound");
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
	Tristate aluKQ0 = aluK.GetQ0Out();
	bool a18 = (romF[PC] & 2) != 0;

	if (!aluKQ0.IsUnknown())
	{
		// ASSUMPTION WARNING
		// we have multiple sources driving Q0... I will assume that they are
		// wired OR and see how that goes
		return aluKQ0.Value() || !a18;
	}

	return !a18;
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
	if (dataIn.IsUnknown())
	{
		aluK.DataIn = aluF.DataIn = aluJ.DataIn = aluE.DataIn = Nybble();
	}
	else
	{
		aluK.DataIn = aluJ.DataIn = (uint8_t)(dataIn.Value() & 0xF);
		aluF.DataIn = aluE.DataIn = (uint8_t)(dataIn.Value() >> 4);
	}
}


void MathBox::SetALUCarryFlags(void)
{
	Timer timer(environment, &totalSetALUCarryTime);

	// the actual carry flags are easy... they just cascade up
	aluK.CarryIn = ((romE[PC] & 1) != 0);
	aluF.CarryIn = aluK.GetCarryOut().Value();
	aluJ.CarryIn = aluF.GetCarryOut().Value();
	aluE.CarryIn = aluJ.GetCarryOut().Value();

	// for our bit shifting flags between ALUs, we find what direction we're
	// shifting from I678, which we set the same for all ALUs, so just pick one
	switch (aluK.I678)
	{
	case 4:
	case 5:
		// downshifting operation
		{
			aluE.SetQ3In(aluK.GetRAM0Out());
			aluJ.SetQ3In(aluE.GetQ0Out());
			aluF.SetQ3In(aluJ.GetQ0Out());
			aluK.SetQ3In(aluF.GetQ0Out());

			bool r15;
			{
				bool E5XORout = aluE.GetOVR() ^ aluE.GetF3();
				bool D4NAND1out = !(E5XORout && ((romE[PC] & 8) != 0));
				r15 = !(D4NAND1out && !((romF[PC] & 2) != 0));
			}
			aluE.SetRAM3In(r15);
			aluJ.SetRAM3In(aluE.GetRAM0Out());
			aluF.SetRAM3In(aluJ.GetRAM0Out());
			aluK.SetRAM3In(aluF.GetRAM0Out());
		}
		break;

	case 6:
	case 7:
		// upshifting operation
		aluK.SetQ0In(GetQ0());
		aluF.SetQ0In(aluK.GetQ3Out());
		aluJ.SetQ0In(aluF.GetQ3Out());
		aluE.SetQ0In(aluJ.GetQ3Out());

		aluK.SetRAM0In(aluE.GetQ3Out());
		aluF.SetRAM0In(aluK.GetRAM3Out());
		aluJ.SetRAM0In(aluF.GetRAM3Out());
		aluE.SetRAM0In(aluJ.GetRAM3Out());
		break;

	default:
		// not doing any shifting at all
		break;
	}
}

void MathBox::SetError(const std::string &_status)
{
	// we only record the first error
	if (error.size() == 0)
		error = _status;
}



