
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
	int AAddress;
	int BAddress;
	int DataIn;
	int I012;
	int I345;
	int I678;
	Tristate RAM0;
	Tristate Q0;
	bool CarryIn;

public:
	bool GetCarryOut(void);
	Tristate GetQ3(void);
	Tristate GetRAM3(void);
	void SetClock(bool state);

private:
	bool clockState;
	bool clockIsSet;
};

#pragma warning(pop)

#endif
