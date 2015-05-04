
#ifndef AM2901_H
#define AM2901_H

#include "Nullable.h"

class ALULogEntry;

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class Am2901
{
public:
	Am2901(void);

public:
	NullableNybble AAddress;
	NullableNybble BAddress;
	NullableNybble DataIn;
	NullableByte I012;
	NullableByte I345;
	NullableByte I678;
	Tristate RAM0;
	Tristate Q0;
	Tristate CarryIn;

public:
	Tristate GetCarryOut(void);
	Tristate GetF3(void) const;
	Tristate GetOVR(void) const;
	Tristate GetQ3(void);
	Tristate GetRAM3(void);
	void SetClock(bool state);

	ALULogEntry GetLogData(void) const;

private:
	NullableNybble GetA(void) const;
	NullableNybble GetB(void) const;
	Tristate GetC3(const NullableNybble &R, const NullableNybble &S) const;
	Tristate GetC4(const NullableNybble &R, const NullableNybble &S) const;
	NullableNybble GetF(void) const;
	NullableNybble GetR(void) const;
	NullableNybble GetS(void) const;
	Tristate GetXORCarry(const NullableNybble &R, const NullableNybble &S);
	Tristate GetXOROverflow(const NullableNybble &R, const NullableNybble &S) const;
	NullableNybble GetRAMValue(const NullableNybble &_address) const;
	void WriteToRAM(const NullableNybble &_address, const NullableNybble &_value);

private:
	Tristate clock;
	NullableNybble ALatch;
	NullableNybble BLatch;
	NullableNybble QLatch;
	NullableNybble RAM[16];

	friend class Am2901TestInterface;
};

#pragma warning(pop)

#endif
