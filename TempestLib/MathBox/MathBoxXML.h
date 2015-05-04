
#ifndef MATHBOXXML_H
#define MATHBOXXML_H

#include "Nullable.h"

class MathBoxXML
{
public:
	static std::string MakeElement(const std::string &name, const NullableByte &value);
	static std::string MakeElement(const std::string &name, const NullableNybble &value);
	static std::string MakeElement(const std::string &name, const Tristate &value);
};


#endif
