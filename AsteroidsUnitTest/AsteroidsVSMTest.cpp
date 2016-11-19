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

      
      TEST_METHOD(TestGO)
      {
         // just set up some short vector commands
         static const uint8_t vectorRAM[] = {
            0x00, 0xF0,
            0x00, 0xF0,
            0x00, 0xF0,
            0x00, 0xF0
         };
         AsteroidsVSM vsm;
         vsm.SetVectorRAM(vectorRAM, sizeof(vectorRAM));

         vsm.Reset();
         Assert::IsTrue(vsm.IsHalted());
         vsm.Go();
         Assert::IsTrue(!vsm.IsHalted());
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