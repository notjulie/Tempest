
#include "stdafx.h"
#include "CppUnitTest.h"

#include "MathBox/MathBox.h"

#include "Am2901TestInterface.h"
#include "MathBoxTestInterface.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MathBoxTest)
{
public:

	TEST_METHOD(TestWrite00To15)
	{
		// create a math box and write 00 to address 0x15
		MathBox mathBox;
		mathBox.Write(0x15, 0x00);

		// ALUF's RAM value at address 7 should be zero
		Am2901 *aluF = MathBoxTestInterface::GetALU(&mathBox, 'F');
		NullableNybble ram7 = Am2901TestInterface::GetRAMValue(aluF, Nybble(0x7));
		Assert::IsFalse(ram7.IsUnknown());
		Assert::AreEqual(ram7.Value().Value(), (uint8_t)0);
	}

};

