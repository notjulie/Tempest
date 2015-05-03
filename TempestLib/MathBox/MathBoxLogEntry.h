
#ifndef MATHBOXLOGENTRY_H
#define MATHBOXLOGENTRY_H

#include "Nullable.h"

class MathBoxLogEntry
{
public:
	NullableByte	PC;

public:
	std::string GetXML(void) const;

private:
	static std::string MakeElement(const std::string &name, const NullableByte &value);
};

#endif
