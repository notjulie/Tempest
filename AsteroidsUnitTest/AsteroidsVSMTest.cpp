#include "stdafx.h"
#include "CppUnitTest.h"

#include "AsteroidsVSM.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AsteroidsUnitTest
{		
	TEST_CLASS(AsteroidsVSMTest)
	{
	public:
      TEST_METHOD(TestReset)
      {
         AsteroidsVSM vsm;
         vsm.Reset();
         Assert::IsTrue(vsm.IsHalted());
      }

		TEST_METHOD(TestHalt)
		{
         static const uint8_t vectorRAM[] = {
            0x00, 0xB0
         };

         AsteroidsVSM vsm;
         vsm.SetVectorRAM(vectorRAM, sizeof(vectorRAM));
         vsm.Interpret();
		}

	};
}