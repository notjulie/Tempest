
#include "stdafx.h"
#include "LS74.h"


LS74::LS74(void)
{
   clock.OnRisingEdge([this](){
      // latch the input on rising edge
      if (_pre && _clr)
         this->Set(d);
   });

   _clr.OnValueChanged([this](bool newValue){
      HandleLoadAndClear();
   });
   _pre.OnValueChanged([this](bool newValue){
      HandleLoadAndClear();
   });
}

void LS74::HandleLoadAndClear(void)
{
   if (!_pre)
      this->Set(true);
   else if (!_clr)
      this->Set(false);
}
