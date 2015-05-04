
#ifndef ALULOGENTRY_H
#define ALULOGENTRY_H

#include "Nullable.h"

class ALULogEntry
{
public:
	NullableByte	I012;
	NullableByte	I345;
	NullableByte	I678;

	NullableNybble	A;
	NullableNybble	B;
	NullableNybble	DataIn;
	NullableNybble	QLatch;
	NullableNybble	R;
	NullableNybble	S;

	NullableNybble	RAM[16];

	Tristate	Cn;
	Tristate	OVR;
	Tristate	F3;

public:
	std::string ToXML(const std::string &name) const;
};

#endif
