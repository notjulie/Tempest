
#include "stdafx.h"
#include "CppUnitTest.h"

#include "Win32/TempestWin32/Win32ComPortStream.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// this file contains tests run using two COM ports connected to each other via NULL modem

namespace TempestWin32Test
{		
	TEST_CLASS(ComPortNullModemTest)
	{
	public:	
		TEST_METHOD(TestMethod1)
		{
         // try writing at gradually increasing rates to test our throughput
         TestWriteSpeed(100);
         TestWriteSpeed(1000);
         TestWriteSpeed(10000);
         TestWriteSpeed(100000);
         TestWriteSpeed(1000000);
      }

   private:
      void TestWriteSpeed(int bytesPerSecond)
      {
         Win32ComPortStream port1("COM6");
         Win32ComPortStream port2("COM7");

         DWORD startTime = GetTickCount();
         int bytesWritten = 0;

         for (;;)
         {
            // see how much time has elapsed
            DWORD elapsed = GetTickCount() - startTime;

            // calculate how many bytes we should have written by now
            int bytesToHaveWritten = bytesPerSecond * elapsed / 1000;

            // write accordingly
            while (bytesWritten < bytesToHaveWritten)
            {
               port1.Write(0);
               ++bytesWritten;
            }

            elapsed = GetTickCount() - startTime;
            if (elapsed > 2000)
               break;
         }
      }

	};
}