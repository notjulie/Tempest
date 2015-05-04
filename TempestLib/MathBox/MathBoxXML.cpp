
#include "stdafx.h"
#include "MathBoxXML.h"


std::string MathBoxXML::MakeElement(const std::string &name, const NullableByte &value)
{
	char buffer[200];
	if (value.IsUnknown())
		sprintf_s(buffer, "<%s value=\"X\"/>", name.c_str());
	else
		sprintf_s(buffer, "<%s value=\"%02X\"/>", name.c_str(), value.Value());
	return buffer;
}

std::string MathBoxXML::MakeElement(const std::string &name, const NullableNybble &value)
{
	char buffer[200];
	if (value.IsUnknown())
		sprintf_s(buffer, "<%s value=\"X\"/>", name.c_str());
	else
		sprintf_s(buffer, "<%s value=\"%X\"/>", name.c_str(), value.Value());
	return buffer;
}

std::string MathBoxXML::MakeElement(const std::string &name, const Tristate &value)
{
	char buffer[200];
	if (value.IsUnknown())
		sprintf_s(buffer, "<%s value=\"X\"/>", name.c_str());
	else
		sprintf_s(buffer, "<%s value=\"%s\"/>", name.c_str(), value.Value() ? "1" : "0");
	return buffer;
}
