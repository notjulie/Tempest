
#ifndef AM2901_H
#define AM2901_H

#include "Tristate.h"

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
	Tristate GetOVR(void);
	Tristate GetQ3(void);
	Tristate GetRAM3(void);
	void SetClock(bool state);

private:
	NullableByte GetB(void);
	NullableByte GetR(void);
	NullableByte GetS(void);
	NullableByte GetRAMValue(const NullableByte &_address);

private:
	Tristate clock;
	NullableByte ALatch;
	NullableByte BLatch;
	NullableByte RAM[16];
};

#pragma warning(pop)

#endif
