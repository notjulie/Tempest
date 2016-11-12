
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

