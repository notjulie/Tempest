
#include "stdafx.h"
#include "LS174.h"


LS174::LS174(void)
{
   clock.OnRisingEdge([this](){
      if (_clear)
         for (int i = 0; i < 8; ++i)
            outputs[i].Set(inputs[i]);
   });

   _clear.OnFallingEdge([this](){
      for (int i = 0; i < 8; ++i)
         outputs[i].Set(false);
   });
}

uint8_t LS174::GetValue(void) const
{
   uint8_t result = 0;
   if (outputs[0])
      result |= 0x01;
   if (outputs[1])
      result |= 0x02;
   if (outputs[2])
      result |= 0x04;
   if (outputs[3])
      result |= 0x08;
   if (outputs[4])
      result |= 0x10;
   if (outputs[5])
      result |= 0x20;
   if (outputs[6])
      result |= 0x40;
   if (outputs[7])
      result |= 0x80;
   return result;
}

