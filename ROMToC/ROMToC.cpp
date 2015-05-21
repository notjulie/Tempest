// ROMToC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdint.h>
#include <vector>

static void WriteROM(FILE *f, const char *s);

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *f = fopen("TempestROMS.c", "wt");
	WriteROM(f, "136002.111");
	WriteROM(f, "136002.112");
	WriteROM(f, "136002.113");
	WriteROM(f, "136002.114");
	WriteROM(f, "136002.115");
	WriteROM(f, "136002.116");
	WriteROM(f, "136002.117");
	WriteROM(f, "136002.118");
	WriteROM(f, "136002.119");
	WriteROM(f, "136002.120");
	WriteROM(f, "136002.121");
	WriteROM(f, "136002.122");
	WriteROM(f, "136002.126");
	WriteROM(f, "136002.127");
	WriteROM(f, "136002.128");
	WriteROM(f, "136002.129");
	WriteROM(f, "136002.130");
	WriteROM(f, "136002.131");
	WriteROM(f, "136002.132");
	fclose(f);

	return 0;
}


static void WriteROM(FILE *f, const char *s)
{
	std::vector<uint8_t> rom;

	FILE *romFile = fopen(s, "rb");
	for (;;)
	{
		int b = getc(romFile);
		if (b < 0)
			break;
		rom.push_back((uint8_t)b);
	}
	fclose(romFile);

	std::string varName = s;
	for (int i = 0; i < varName.size(); ++i)
		if (varName[i] == '.')
			varName[i] = '_';

	fprintf(f, "\n\n\n");
	fprintf(f, "const uint8_t %s[%d] = {\n", varName.c_str(), rom.size());
	for (int i = 0; i < rom.size(); ++i)
	{
		if (i != rom.size() - 1)
		{
			fprintf(f, "    0x%02X,", rom[i]);
			if ((i&7) == 7)
				fprintf(f, "\n");
		}
		else
		{
			fprintf(f, "    0x%02X\n", rom[i]);
		}
	}
	fprintf(f, "};\n\n\n");
}

