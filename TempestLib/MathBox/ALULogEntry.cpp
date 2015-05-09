
#include "stdafx.h"
#include "MathBoxXML.h"
#include "ALULogEntry.h"

std::string ALULogEntry::ToXML(const std::string &name) const
{
	std::string result;

	result += "<ALU Name=\"";
	result += name;
	result += "\">";

	result += MathBoxXML::MakeElement("A", A);
	result += MathBoxXML::MakeElement("B", B);
	result += MathBoxXML::MakeElement("Cn", Cn);
	result += MathBoxXML::MakeElement("DataIn", DataIn);
	result += MathBoxXML::MakeElement("F3", F3);
	result += MathBoxXML::MakeElement("I012", I012);
	result += MathBoxXML::MakeElement("I345", I345);
	result += MathBoxXML::MakeElement("I678", I678);
	result += MathBoxXML::MakeElement("OVR", OVR);
	result += MathBoxXML::MakeElement("QLatch", QLatch);
	result += MathBoxXML::MakeElement("R", R);
	result += MathBoxXML::MakeElement("S", S);

	result += MathBoxXML::MakeElement("RAM0", RAM[0]);
	result += MathBoxXML::MakeElement("RAM1", RAM[1]);
	result += MathBoxXML::MakeElement("RAM2", RAM[2]);
	result += MathBoxXML::MakeElement("RAM3", RAM[3]);
	result += MathBoxXML::MakeElement("RAM4", RAM[4]);
	result += MathBoxXML::MakeElement("RAM5", RAM[5]);
	result += MathBoxXML::MakeElement("RAM6", RAM[6]);
	result += MathBoxXML::MakeElement("RAM7", RAM[7]);
	result += MathBoxXML::MakeElement("RAM8", RAM[8]);
	result += MathBoxXML::MakeElement("RAM9", RAM[9]);
	result += MathBoxXML::MakeElement("RAMA", RAM[10]);
	result += MathBoxXML::MakeElement("RAMB", RAM[11]);
	result += MathBoxXML::MakeElement("RAMC", RAM[12]);
	result += MathBoxXML::MakeElement("RAMD", RAM[13]);
	result += MathBoxXML::MakeElement("RAME", RAM[14]);
	result += MathBoxXML::MakeElement("RAMF", RAM[15]);

	result += "</ALU>";
	return result;
}

