
#include "stdafx.h"
#include "LS109.h"


LS109::LS109(void)
{
   clock.OnRisingEdge([this](){
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
}
