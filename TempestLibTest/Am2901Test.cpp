#include "stdafx.h"
#include "CppUnitTest.h"

#include "MathBox/Am2901.h"

#include "Am2901TestInterface.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Am2901Test)
{
public:
		
	TEST_METHOD(TestWriteFToRAM)
	{
		Am2901	alu;

		// set the clock low as the idle state
		alu.SetClock(false);

		// the rising edge happens before we put the new data on the lines
		alu.SetClock(true);

		// set the inputs
		alu.AAddress = 0x1;
		alu.BAddress = 0x7;
		alu.CarryIn = true;
		alu.DataIn = 0x0;
		alu.I012 = 0x7;
		alu.I345 = 0x3;
		alu.I678 = 0x3;

		// lowering the clock tells the ALU to start doing its mathy stuff
		alu.SetClock(false);

		// then on the rising edge it writes to RAM if commanded
		alu.SetClock(true);

		// that should clear memory address 7
		Nybble ram7 = Am2901TestInterface::GetRAMValue(&alu, Nybble(0x7));
		Assert::AreEqual(ram7.Value(), (uint8_t)0);
	}
};
