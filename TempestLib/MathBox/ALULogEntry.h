
#ifndef ALULOGENTRY_H
#define ALULOGENTRY_H

#include "Nullable.h"

class ALULogEntry
{
public:
	NullableByte	I012;
	NullableByte	I345;
	NullableByte	I678;

	NullableNybble	R;
	NullableNybble	S;

	Tristate	Cn;
	Tristate	OVR;
	Tristate	F3;

public:
	std::string ToXML(const std::string &name) const;
};

#endif
