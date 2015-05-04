
#ifndef MATHBOXLOGENTRY_H
#define MATHBOXLOGENTRY_H

#include "ALULogEntry.h"
#include "Nullable.h"

class MathBoxLogEntry
{
public:
	Tristate			BEGIN;
	Tristate			J;
	Tristate			PCEN;
	Tristate			S;
	Tristate			S0;
	Tristate			S1;

	NullableByte	PC;

	ALULogEntry		ALUE;
	ALULogEntry		ALUF;
	ALULogEntry		ALUJ;
	ALULogEntry		ALUK;

public:
	std::string GetXML(void) const;
};

#endif
