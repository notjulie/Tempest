// MathBoxDisassembler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"

class MathBoxDisassembler
{
public:
	MathBoxDisassembler(void);
	void Disassemble(void);

private:
	void DisassembleInstruction(int address);
	void DisplayEntryPoints(const std::vector<uint8_t> addresses);
	void DisplayJumpInstructionInformation(int address);

private:
	static void LoadROM(int resID, std::vector<uint8_t> &rom);
	static void LoadROM(std::vector<uint8_t> &rom, const void *p, int length);

private:
	std::vector<uint8_t>	romA;
	std::vector<uint8_t>	romE;
	std::vector<uint8_t>	romF;
	std::vector<uint8_t>	romH;
	std::vector<uint8_t>	romJ;
	std::vector<uint8_t>	romK;
	std::vector<uint8_t>	romL;
};


MathBoxDisassembler::MathBoxDisassembler(void)
{
	LoadROM(IDR_ROM_A, romA);
	LoadROM(IDR_ROM_E, romE);
	LoadROM(IDR_ROM_F, romF);
	LoadROM(IDR_ROM_H, romH);
	LoadROM(IDR_ROM_J, romJ);
	LoadROM(IDR_ROM_K, romK);
	LoadROM(IDR_ROM_L, romL);
}

void MathBoxDisassembler::Disassemble(void)
{
	for (int i = 0; i < 256; ++i)
		DisassembleInstruction(i);
}

void MathBoxDisassembler::DisassembleInstruction(int address)
{
	// first thing we want to do is note if this instruction is an entry point
	// from the CPU
	std::vector<uint8_t> srcAddresses;
	for (int i = 0; i < romA.size(); ++i)
		if (romA[i] == address)
			srcAddresses.push_back(i);
	if (srcAddresses.size() != 0)
		DisplayEntryPoints(srcAddresses);

	// display the address
	printf("%02X:\n", address);

	// if this is a jump instruction, note it as such
	if ((romE[address] & 4) != 0)
	{
		DisplayJumpInstructionInformation(address);
	}
	else if ((romF[address] & 8) != 0)
	{
		// if this is not a jump instruction but we are loading the address latch
		// we should note that
		printf("Address latch <== %02X\n", ((romL[address] << 4) + romK[address]));
	}

	// lastly we note if this is a stop point or an unconditional jump
	if ((romH[address] & 8) != 0)
	{
		printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		printf("\n");
		printf("\n");
	}
	else if ((romE[address] & 4) != 0 && (romE[address] & 8) == 0)
	{
		printf("-------------------------------------------------\n");
		printf("-------------------------------------------------\n");
		printf("\n");
		printf("\n");
	}
}


void MathBoxDisassembler::DisplayEntryPoints(const std::vector<uint8_t> addresses)
{
	printf("\n\n");

	for (int i = 0; i < addresses.size(); ++i)
		printf("   ");
	printf("              \\\\\n");

	for (int i = 0; i < addresses.size(); ++i)
		printf("%02X ", addresses[i]);
	printf("================\n");

	for (int i = 0; i < addresses.size(); ++i)
		printf("   ");
	printf("              //\n");
}


void MathBoxDisassembler::DisplayJumpInstructionInformation(int address)
{
	bool ldab = (romF[address] & 8) != 0;
	bool s = (romE[address] & 8) != 0;

	if (s)
		printf("Conditional ");
	printf("Jump: ");

	if (ldab)
	{
		printf("%02X\n", (uint8_t)((romL[address] << 4) + romK[address]));
	}
	else
	{
		printf("Previously loaded address\n");
	}
}

void MathBoxDisassembler::LoadROM(int id, std::vector<uint8_t> &rom)
{
	// load the ROM resource
	HMODULE dllModule = GetModuleHandle(NULL);
	if (dllModule == NULL)
		throw std::runtime_error("Error loading ROM resources, GetModuleHandle failed");

	HRSRC rsrc = FindResource(dllModule, MAKEINTRESOURCE(id), "ROM");
	if (rsrc == NULL)
		std::runtime_error("Error loading ROM resources, FindResourceFailed");

	HGLOBAL h = LoadResource(dllModule, rsrc);
	if (h == NULL)
		std::runtime_error("Error loading ROM resources, LoadResource failed");

	LoadROM(rom, LockResource(h), SizeofResource(dllModule, rsrc));

	FreeResource(h);
}


void MathBoxDisassembler::LoadROM(std::vector<uint8_t> &rom, const void *p, int length)
{
	rom.resize(length);
	for (int i = 0; i < length; ++i)
		rom[i] = ((const uint8_t *)p)[i];
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		MathBoxDisassembler	disassembler;
		disassembler.Disassemble();
	}
	catch (std::exception &x)
	{
		printf("%s\n", x.what());
	}

	getchar();
	return 0;
}

