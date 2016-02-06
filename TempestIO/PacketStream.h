
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
   enum ReadState {
      ReadIdle,
      InPacket,
      Escape
   };

private:
   AbstractTempestStream *stream;
   ReadState readState;
};

#endif
