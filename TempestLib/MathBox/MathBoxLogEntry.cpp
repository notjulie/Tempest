
#include "stdafx.h"
#include "MathBoxXML.h"
#include "MathBoxLogEntry.h"

std::string MathBoxLogEntry::GetXML(void) const
{
	std::string result;
	result += "<Entry>";

	// add bits
	result += MathBoxXML::MakeElement("BEGIN", BEGIN);
	result += MathBoxXML::MakeElement("J", J);
	result += MathBoxXML::MakeElement("PCEN", PCEN);
	result += MathBoxXML::MakeElement("S", S);
	result += MathBoxXML::MakeElement("S0", S0);
	result += MathBoxXML::MakeElement("S1", S1);

	// add bytes
	result += MathBoxXML::MakeElement("AddressIn", AddressIn);
	result += MathBoxXML::MakeElement("DataIn", DataIn);
	result += MathBoxXML::MakeElement("PC", PC);
	result += MathBoxXML::MakeElement("JumpLatch", JumpLatch);

	// add the ALUs
	result += ALUE.ToXML("E");
	result += ALUF.ToXML("F");
	result += ALUJ.ToXML("J");
	result += ALUK.ToXML("K");

	result += "</Entry>";
	return result;
}

