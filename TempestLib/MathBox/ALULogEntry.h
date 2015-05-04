
#ifndef ALULOGENTRY_H
#define ALULOGENTRY_H

#include "Nullable.h"

class ALULogEntry
{
public:
	Tristate	OVR;
	Tristate	F3;

public:
	std::string ToXML(const std::string &name) const;
};

#endif
