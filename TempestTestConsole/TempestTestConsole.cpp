// TempestTestConsole.cpp : Defines the entry point for the console application.
//
/*
* File:   main.cpp
* Author: Randy
*
* Created on April 25, 2015, 1:46 PM
*/

#include <cstdlib>
#include <exception>
#include <string>
#include <stdio.h>

#include "TempestLib/6502/CPU6502.h"
#include "TempestLib/TempestBus.h"
#include "TempestLib/TempestException.h"
#include "TempestLib/Win32/Win32PerformanceCounter3KHzClock.h"

using namespace std;

/*
*
*/
int main(int argc, char** argv) {
	Win32PerformanceCounter3KHzClock clock3KHz;

	TempestBus  bus(&clock3KHz);
	CPU6502     cpu(&bus);

	try
	{
		// create the bus's ROM
		const std::string romPath("C:\\Users\\Randy\\Documents\\NetBeansProjects\\Tempest\\tempest_ver1_roms\\");
		bus.LoadROM(romPath + "136002.113", 0x9000);
		bus.LoadROM(romPath + "136002.114", 0x9800);
		bus.LoadROM(romPath + "136002.115", 0xA000);
		bus.LoadROM(romPath + "136002.116", 0xA800);
		bus.LoadROM(romPath + "136002.117", 0xB000);
		bus.LoadROM(romPath + "136002.118", 0xB800);
		bus.LoadROM(romPath + "136002.119", 0xC000);
		bus.LoadROM(romPath + "136002.120", 0xC800);
		bus.LoadROM(romPath + "136002.121", 0xD000);
		bus.LoadROM(romPath + "136002.122", 0xD800);

		// load the Vector ROM
		bus.LoadROM(romPath + "136002.111", 0x3000);
		bus.LoadROM(romPath + "136002.112", 0x3800);

		// run
		cpu.Reset();
		cpu.Run();
	}
	catch (TempestException &x)
	{
		printf("%s\n", x.what());
	}
	catch (...)
	{
		printf("Unknown exception\n");
	}

	cpu.GetInstructionLog().Print();

	getchar();
	return 0;
}

