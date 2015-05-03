
#include "stdafx.h"
#include "MathBoxLogEntry.h"

std::string MathBoxLogEntry::GetXML(void) const
{
	std::string result;
	result += "<Entry>";

	// add bits
	result += MakeElement("BEGIN", BEGIN);
	result += MakeElement("J", J);
	result += MakeElement("PCEN", PCEN);
	result += MakeElement("S", S);
	result += MakeElement("S0", S0);
	result += MakeElement("S1", S1);

	// add bytes
	result += MakeElement("PC", PC);

	result += "</Entry>";
	return result;
}

std::string MathBoxLogEntry::MakeElement(const std::string &name, const NullableByte &value)
{
	char buffer[200];
	if (value.IsUnknown())
		sprintf_s(buffer, "<%s value=\"X\"/>", name.c_str());
	else
		sprintf_s(buffer, "<%s value=\"%02X\"/>", name.c_str(), value.Value());
	return buffer;
}

std::string MathBoxLogEntry::MakeElement(const std::string &name, const Tristate &value)
{
	char buffer[200];
	if (value.IsUnknown())
		sprintf_s(buffer, "<%s value=\"X\"/>", name.c_str());
	else
		sprintf_s(buffer, "<%s value=\"%s\"/>", name.c_str(), value.Value() ? "1" : "0");
	return buffer;
}
