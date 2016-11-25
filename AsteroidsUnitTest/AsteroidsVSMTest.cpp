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


      TEST_METHOD(TestJump)
      {
         // jump to a halt instruction, verify that the PC is what we expect
         uint8_t vectorRAM[2000];
         uint16_t jumpAddress = 500; // word address, not byte address

         vectorRAM[0] = (uint8_t)jumpAddress;
         vectorRAM[1] = 0xE0 + (jumpAddress >> 8);
         vectorRAM[jumpAddress * 2] = 0;
         vectorRAM[jumpAddress * 2 + 1] = 0xB0;

         AsteroidsVSM vsm;
         vsm.SetVectorRAM(vectorRAM, sizeof(vectorRAM));
         vsm.Interpret();
         Assert::IsTrue(vsm.GetPCWordAddress() == jumpAddress+1);
      }
   };
}