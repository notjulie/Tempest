
#ifndef AM2901_H
#define AM2901_H

#include "Am2901Tables.h"

class ALULogEntry;
class MathBoxTracer;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class Am2901
{
public:
	Am2901(void);

public:
	Nybble AAddress;
	Nybble BAddress;
	Nybble DataIn;
	uint8_t I012;
	uint8_t I345;
	uint8_t I678;
	bool CarryIn;
	bool Q0In;
	bool Q3In;
	bool RAM0In;
	bool RAM3In;

public:
	bool GetCarryOut(void);
	bool GetF3(void) const;
	bool GetOVR(void) const;
	bool GetQ0Out(void);
	bool GetQ3Out(void);
	bool GetRAM0Out(void);
	bool GetRAM3Out(void);

	Nybble GetY(void);

	void SetClock(bool state);

private:
	Nybble GetA(void) const;
	Nybble GetB(void) const;
	Nybble GetF(void) const;
	Nybble GetR(void) const;
	Nybble GetS(void) const;
	bool GetXORCarry(Nybble R, Nybble S) const;
	bool GetXOROverflow(Nybble R, Nybble S) const;

private:
	static Am2901Tables Tables;

private:
	bool clock;

	Nybble ALatch;
	Nybble BLatch;
	Nybble QLatch;
	Nybble RAM[16];

	friend class Am2901TestInterface;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
