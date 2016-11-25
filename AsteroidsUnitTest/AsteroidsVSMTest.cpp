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
         Assert::IsTrue(vsm.GetPCWordAddress() == jumpAddress + 1);
      }


      TEST_METHOD(TestJSR)
      {
         // jump to an RTS, only to return to a halt instruction, verify that
         // the PC is what we expect
         uint8_t vectorRAM[2000];
         uint16_t jumpAddress = 500; // word address, not byte address

         vectorRAM[0] = (uint8_t)jumpAddress;
         vectorRAM[1] = 0xC0 + (jumpAddress >> 8);
         vectorRAM[2] = 0;
         vectorRAM[3] = 0xB0;
         vectorRAM[jumpAddress * 2] = 0;
         vectorRAM[jumpAddress * 2 + 1] = 0xD0;

         AsteroidsVSM vsm;
         vsm.SetVectorRAM(vectorRAM, sizeof(vectorRAM));
         vsm.Interpret();
         Assert::IsTrue(vsm.GetPCWordAddress() == 2);
      }


      TEST_METHOD(TestJSRNesting)
      {
         // jump to some nested JSRs, only to return to a halt instruction, verify that
         // the PC is what we expect
         uint8_t vectorRAM[2000];
         uint16_t sub1Address = 100; // word address, not byte address
         uint16_t sub2Address = 200; // word address, not byte address
         uint16_t sub3Address = 300; // word address, not byte address
         uint16_t sub4Address = 400; // word address, not byte address

         vectorRAM[0] = (uint8_t)sub1Address;
         vectorRAM[1] = 0xC0 + (sub1Address >> 8);
         vectorRAM[2] = 0;
         vectorRAM[3] = 0xB0;

         vectorRAM[sub1Address * 2] = (uint8_t)sub2Address;
         vectorRAM[sub1Address * 2 + 1] = 0xC0 + (sub2Address >> 8);
         vectorRAM[sub1Address * 2 + 2] = 0;
         vectorRAM[sub1Address * 2 + 3] = 0xD0;

         vectorRAM[sub2Address * 2] = (uint8_t)sub3Address;
         vectorRAM[sub2Address * 2 + 1] = 0xC0 + (sub3Address >> 8);
         vectorRAM[sub2Address * 2 + 2] = 0;
         vectorRAM[sub2Address * 2 + 3] = 0xD0;

         vectorRAM[sub3Address * 2] = (uint8_t)sub4Address;
         vectorRAM[sub3Address * 2 + 1] = 0xC0 + (sub4Address >> 8);
         vectorRAM[sub3Address * 2 + 2] = 0;
         vectorRAM[sub3Address * 2 + 3] = 0xD0;

         vectorRAM[sub4Address * 2 + 0] = 0;
         vectorRAM[sub4Address * 2 + 1] = 0xD0;

         AsteroidsVSM vsm;
         vsm.SetVectorRAM(vectorRAM, sizeof(vectorRAM));
         vsm.Interpret();
         Assert::IsTrue(vsm.GetPCWordAddress() == 2);
      }
   };
}
