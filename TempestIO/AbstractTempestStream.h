
#ifndef ABSTRACTTEMPESTSTREAM_H
#define ABSTRACTTEMPESTSTREAM_H

class AbstractTempestStream
{
public:
   virtual ~AbstractTempestStream(void) {}
   virtual int  Peek(void) = 0;
   virtual int  Read(void) = 0;
   virtual void Write(uint8_t b) = 0;

   void WriteString(const char *s);
};

#endif
