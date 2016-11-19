
#include "stdafx.h"
#include "LS109.h"


LS109::LS109(void)
{
   // add our hooks
   clock.OnRisingEdge([this](){
      // never mind if we are in preset or clear
      if (!_pre || !_clr)
         return;

      // update according to J & K inputs
      if (!j && !k)
         q.Set(false);
      else if (j && !k)
         q.Set(!q);
      else if (j && k)
         q.Set(true);
      _q.Set(!q);
   });

   _pre.OnFallingEdge([this](){
      q.Set(true);
      _q.Set(!q);
   });

   _clr.OnFallingEdge([this](){
      q.Set(!_pre);
      _q.Set(!q);
   });

   // set our initial state
   _pre.Set(true);
   _clr.Set(true);
}
