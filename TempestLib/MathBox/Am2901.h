
#ifndef AM2901_H
#define AM2901_H

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
	NullableNybble AAddress;
	NullableNybble BAddress;
	NullableNybble DataIn;
	NullableByte I012;
	NullableByte I345;
	NullableByte I678;
	Tristate CarryIn;

public:
	void ClearRAM(const NullableNybble &value);
	Tristate GetCarryOut(void);
	Tristate GetF3(void) const;
	Tristate GetOVR(void) const;
	Tristate GetQ0Out(void);
	Tristate GetQ3Out(void);
	Tristate GetRAM0Out(void);
	Tristate GetRAM3Out(void);

	void SetClock(bool state);
	void SetQ0In(const Tristate &q0) { Q0In = q0; }
	void SetQ3In(const Tristate &q3) { Q3In = q3; }
	void SetRAM0In(const Tristate &ram0) { RAM0In = ram0; }
	void SetRAM3In(const Tristate &ram3) { RAM3In = ram3; }

	ALULogEntry GetLogData(void) const;

	void EnableTrace(MathBoxTracer *_tracer) { tracer = _tracer; }

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
	void Trace(const std::string &context);
	void TraceValue(const std::string &name, const NullableByte &value);
	void TraceValue(const std::string &name, const NullableNybble &value);
	void TraceValue(const std::string &name, const Tristate &value);

private:
	Tristate clock;
	Tristate Q0In;
	Tristate Q3In;
	Tristate RAM0In;
	Tristate RAM3In;

	NullableNybble ALatch;
	NullableNybble BLatch;
	NullableNybble QLatch;
	NullableNybble RAM[16];

	MathBoxTracer *tracer;

	friend class Am2901TestInterface;
};

#pragma warning(pop)

#endif
