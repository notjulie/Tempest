
#include <stdint.h>
#include <stdio.h>

#include "TempestException.h"

#include "MathBox.h"


uint8_t MathBox::GetStatus(void)
{
   throw TempestException("MathBox::GetStatus not implemented");
}

uint8_t MathBox::Read1(void)
{
   throw TempestException("MathBox::Read1 not implemented");
}

uint8_t MathBox::Read2(void)
{
   throw TempestException("MathBox::Read2 not implemented");
}

uint8_t MathBox::Write(uint8_t address, uint8_t value)
{
   char buffer[200];
   sprintf(buffer, "MathBox::Write unsupported address: 0x%X", address);
   throw TempestException(buffer);
}
