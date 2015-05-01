
#ifndef AM2901_H
#define AM2901_H

#include <stdint.h>

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
	bool RAM0;
	bool Q0;
	bool CarryIn;

public:
	bool GetCarryOut(void);
	bool GetQ3(void);
	bool GetRAM3(void);
	void SetClock(bool state);

private:
	bool clockState;
	bool clockIsSet;
};

#endif
