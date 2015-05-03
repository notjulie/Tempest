
#ifndef MATHBOXLOGENTRY_H
#define MATHBOXLOGENTRY_H

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

public:
	std::string GetXML(void) const;

private:
	static std::string MakeElement(const std::string &name, const NullableByte &value);
	static std::string MakeElement(const std::string &name, const Tristate &value);
};

#endif
