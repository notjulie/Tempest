
#include "stdafx.h"
#include "LS74.h"


LS74::LS74(void)
{
   clock.OnRisingEdge([this](){
      // latch the input on rising edge
      if (_pre && _clr)
         q.Set(d);
      _q.Set(!q);
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
      q.Set(true);
   else if (!_clr)
      q.Set(false);
   _q.Set(!q);
}
