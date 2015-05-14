
#ifndef AM2901_H
#define AM2901_H

#include "Am2901Tables.h"
#include "Nullable.h"

class ALULogEntry;
class MathBoxTracer;

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

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

public:
	Tristate GetCarryOut(void);
	bool GetF3(void) const;
	bool GetOVR(void) const;
	bool GetQ0Out(void);
	bool GetQ3Out(void);
	bool GetRAM0Out(void);
	bool GetRAM3Out(void);

	Nybble GetY(void);

	void SetClock(bool state);
	void SetQ0In(const Tristate &q0) { Q0In = q0; }
	void SetQ3In(const Tristate &q3) { Q3In = q3; }
	void SetRAM0In(const Tristate &ram0) { RAM0In = ram0; }
	void SetRAM3In(const Tristate &ram3) { RAM3In = ram3; }

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
	Tristate Q0In;
	Tristate Q3In;
	Tristate RAM0In;
	Tristate RAM3In;

	Nybble ALatch;
	Nybble BLatch;
	Nybble QLatch;
	Nybble RAM[16];

	friend class Am2901TestInterface;
};

#pragma warning(pop)

#endif
