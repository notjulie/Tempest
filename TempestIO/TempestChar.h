
#ifndef TEMPESTCHAR_H
#define TEMPESTCHAR_H


class TempestChar {
public:
   uint8_t GetRawValue(void) const;
   char ToAscii(void) const;
   static TempestChar FromAscii(char value);
   static TempestChar FromRawValue(uint8_t value);

private:
   uint8_t value = 0;
};


#endif
