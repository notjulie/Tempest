
#ifndef PACKETSTREAM_H
#define PACKETSTREAM_H

class AbstractTempestStream;

class PacketStream {
public:
   PacketStream(AbstractTempestStream *_stream);

   void EndPacket(void);
   void StartPacket(void);
   void Write(uint8_t b);
   int Read(void);

private:
   bool VerifyIncomingPacket(void);

private:
   enum ReadState {
      ReadIdle,
      InPacket,
      Escape,
      HavePacket
   };

private:
   AbstractTempestStream *stream;
   ReadState readState;
   uint8_t outgoingPacketLength;
   uint8_t outgoingPacketCheckSum;
   int incomingPacketLength;
   uint8_t incomingPacket[2048];
   int incomingPacketReadIndex;
};

#endif
