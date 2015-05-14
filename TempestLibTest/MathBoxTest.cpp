
#include "stdafx.h"
#include "CppUnitTest.h"

#include "MathBox/MathBox.h"
#include "MathBox/MathBoxTracer.h"

#include "Am2901TestInterface.h"
#include "MathBoxTestInterface.h"
#include "resource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MathBoxTest) 
{
private:
	class Tracer : public MathBoxTracer
	{
	public:
		virtual void Trace(const std::string &s) {
			::OutputDebugString(s.c_str());
		}
	};

private:
	Tracer	tracer;

public:

	TEST_METHOD(TestWrite00To15)
	{
		// create a math box and write 00 to address 0x15
		MathBox mathBox;
		LoadROMs(&mathBox);
		mathBox.Write(0x15, 0x00);

		// ALUF's RAM value at address 7 should be zero
		Am2901 *aluF = MathBoxTestInterface::GetALU(&mathBox, 'F');
		NullableNybble ram7 = Am2901TestInterface::GetRAMValue(aluF, Nybble(0x7));
		Assert::IsFalse(ram7.IsUnknown());
		Assert::AreEqual(ram7.Value().Value(), (uint8_t)0);
	}

private:
	static void LoadROMs(MathBox *mathBox)
	{
		LoadROM(mathBox, IDR_ROM_A, 'A');
		LoadROM(mathBox, IDR_ROM_E, 'E');
		LoadROM(mathBox, IDR_ROM_F, 'F');
		LoadROM(mathBox, IDR_ROM_H, 'H');
		LoadROM(mathBox, IDR_ROM_J, 'J');
		LoadROM(mathBox, IDR_ROM_K, 'K');
		LoadROM(mathBox, IDR_ROM_L, 'L');
	}

	static void LoadROM(MathBox *mathBox, int id, char slot)
	{
		// load the ROM resource
		HMODULE dllModule = GetModuleHandle("TempestLibTest");
		if (dllModule == NULL)
			Assert::Fail(L"Error loading ROM resources, GetModuleHandle failed");

		HRSRC rsrc = FindResource(dllModule, MAKEINTRESOURCE(id), "ROM");
		if (rsrc == NULL)
			Assert::Fail(L"Error loading ROM resources, FindResourceFailed");

		HGLOBAL h = LoadResource(dllModule, rsrc);
		if (h == NULL)
			Assert::Fail(L"Error loading ROM resources, LoadResource failed");

		mathBox->LoadROM((const uint8_t *)LockResource(h), SizeofResource(dllModule, rsrc), slot);

		FreeResource(h);
	}
};

