
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
	NullableByte DataIn;
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
	NullableByte GetA(void);
	NullableByte GetB(void);
	Tristate GetC3(const NullableByte &R, const NullableByte &S);
	Tristate GetC4(const NullableByte &R, const NullableByte &S);
	NullableByte GetF(void);
	NullableByte GetR(void);
	NullableByte GetS(void);
	Tristate GetXORCarry(const NullableByte &R, const NullableByte &S);
	Tristate GetXOROverflow(const NullableByte &R, const NullableByte &S);
	NullableByte GetRAMValue(const NullableByte &_address);
	void WriteToRAM(const NullableByte &_address, const NullableByte &_value);

private:
	Tristate clock;
	NullableByte ALatch;
	NullableByte BLatch;
	NullableByte QLatch;
	NullableByte RAM[16];
};

#pragma warning(pop)

#endif
