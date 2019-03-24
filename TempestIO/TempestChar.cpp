
#include "TempestIO.Headers.h"
#include "VectorGameException.h"
#include "TempestChar.h"

// this is a phony value for now... I don't know the exact value but this works for
// all purposes that this class serves for now
#define SPACE_VALUE 255

TempestChar TempestChar::FromRawValue(uint8_t value)
{
   TempestChar result;
   result.value = value;
   return result;
}

TempestChar TempestChar::FromAscii(char value)
{
   TempestChar result;
   if (value >= '0' && value <= '9')
      result.value = value - '0';
   else if (value >= 'A' && value <= 'Z')
      result.value = 10 + (value - 'A');
   else if (value == ' ')
      result.value = SPACE_VALUE;
   else
      throw VectorGameException("TempestChar::FromAscii: don't know this one");
   return result;
}

uint8_t TempestChar::GetRawValue(void) const
{
   return value;
}

char TempestChar::ToAscii(void) const
{
   if (value < 10)
      return '0' + value;
   else if (value < 36)
      return 'A' + (value - 10);
   else if (value == SPACE_VALUE)
      return ' ';
   else
      throw VectorGameException("TempestChar::ToAscii: don't know this one");
}

bool TempestChar::operator!=(const TempestChar &c) const
{
   return value != c.value;
}
