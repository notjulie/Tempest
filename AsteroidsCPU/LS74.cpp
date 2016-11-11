
#include "stdafx.h"
#include "LS74.h"


LS74::LS74(void)
{
   clock.OnRisingEdge([this](){
      // latch the input on rising edge
      if (_pre && _clr)
         q.Set(d);
   });

   _clr.OnValueChanged([this](bool newValue){
      HandleLoadAndClear();
   });
   _pre.OnValueChanged([this](bool newValue){
      HandleLoadAndClear();
   });

   // connect our non-inverted output to the inverted output
   _q.SetSource(q);
}

void LS74::HandleLoadAndClear(void)
{
   if (!_pre)
      q.Set(true);
   else if (!_clr)
      q.Set(false);
}
