
#ifndef ABSTRACTTEMPESTSTREAM_H
#define ABSTRACTTEMPESTSTREAM_H

class AbstractTempestStream
{
public:
   virtual int  Read(void) = 0;
   virtual void Write(uint8_t b) = 0;
};

#endif
