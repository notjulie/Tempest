
#ifndef AM2901_H
#define AM2901_H

#include "Nullable.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class Am2901
{
public:
	Am2901(void);

public:
	NullableByte AAddress;
	NullableByte BAddress;
	NullableNybble DataIn;
	NullableByte I012;
	NullableByte I345;
	NullableByte I678;
	Tristate RAM0;
	Tristate Q0;
	Tristate CarryIn;

public:
	Tristate GetCarryOut(void);
	Tristate GetF3(void);
	Tristate GetOVR(void);
	Tristate GetQ3(void);
	Tristate GetRAM3(void);
	void SetClock(bool state);

private:
	NullableNybble GetA(void);
	NullableNybble GetB(void);
	Tristate GetC3(const NullableNybble &R, const NullableNybble &S);
	Tristate GetC4(const NullableNybble &R, const NullableNybble &S);
	NullableNybble GetF(void);
	NullableNybble GetR(void);
	NullableNybble GetS(void);
	Tristate GetXORCarry(const NullableNybble &R, const NullableNybble &S);
	Tristate GetXOROverflow(const NullableNybble &R, const NullableNybble &S);
	NullableNybble GetRAMValue(const NullableByte &_address);
	void WriteToRAM(const NullableByte &_address, const NullableNybble &_value);

private:
	Tristate clock;
	NullableNybble ALatch;
	NullableNybble BLatch;
	NullableNybble QLatch;
	NullableNybble RAM[16];
};

#pragma warning(pop)

#endif
