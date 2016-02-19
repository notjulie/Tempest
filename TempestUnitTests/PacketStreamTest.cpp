#include "stdafx.h"

#include "../TempestIO/PacketStream.h"
#include "../TempestIO/TempestMemoryStream.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TempestUnitTests
{		
	TEST_CLASS(PacketStreamTest)
	{
	public:
		
		TEST_METHOD(TestPackets)
		{
         {
            uint8_t packet[] = { 0x01, 0x02, 0x03 };
            TestSendAndReceive(packet, sizeof(packet));
         }
      
         {
            uint8_t packet[] = { 0x01, 0x02, 0xFD, 0xFE, 0xFF };
            TestSendAndReceive(packet, sizeof(packet));
         }
      }


   private:
      void TestSendAndReceive(const uint8_t *packet, int length)
      {
         TempestMemoryStream stream;

         PacketStream outStream(stream.GetLeftSide());
         outStream.StartPacket();
         for (int i = 0; i < length; ++i)
            outStream.Write(packet[i]);
         outStream.EndPacket();

         PacketStream inStream(stream.GetRightSide());
         std::vector<uint8_t> inPacket;
         inPacket.resize(length);
         int bytesRead = inStream.ReadPacket(&inPacket[0], inPacket.size());

         Assert::AreEqual(length, bytesRead, L"Received packet size correct");

         for (int i = 0; i < length; ++i)
            Assert::AreEqual(packet[i], inPacket[i]);
      }
	};
}
