
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
         // do a binary search to estimate how fast we can write...
         int highestSuccess = 0;
         int lowestFailure = 0;

         // we can't start our binary search until we have a failure... do increasing
         // values until we get a failure
         int i = 100;
         for (;;)
         {
            try
            {
               TestWriteSpeed(i);
               highestSuccess = i;
               i *= 100;
            }
            catch (...)
            {
               lowestFailure = i;
               break;
            }
         }

         // now do our binary search
         while (lowestFailure - highestSuccess > 10)
         {
            int testValue = (lowestFailure + highestSuccess) / 2;
            try
            {
               TestWriteSpeed(testValue);
               highestSuccess = testValue;
            }
            catch (...)
            {
               lowestFailure = testValue;
            }
         }

         Assert::AreEqual(0, (lowestFailure + highestSuccess) / 2);
      }

   private:
      void TestWriteSpeed(int bytesPerSecond)
      {
         Win32ComPortStream port1("COM3");
         Win32ComPortStream port2("CNCB2");

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