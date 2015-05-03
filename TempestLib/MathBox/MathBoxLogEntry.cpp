
#include "stdafx.h"
#include "MathBoxLogEntry.h"

std::string MathBoxLogEntry::GetXML(void) const
{
	std::string result;
	result += "<Entry>";
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

