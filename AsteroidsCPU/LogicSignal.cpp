
#include "stdafx.h"
#include "LogicSignal.h"


// =========================================================
// ===================== LogicOutput =======================
// =========================================================

void LogicOutput::SetOutputValue(bool value)
{
   // never mind no-ops
   if (value == this->value)
      return;

   // set and notify
   this->value = value;
   for (auto callback : changeCallbacks)
      callback(value);
   if (value)
   {
      for (auto callback : risingEdgeCallbacks)
         callback();
   }
   else
   {
      for (auto callback : fallingEdgeCallbacks)
         callback();
   }
}


// =========================================================
// ==================== LogicSignal ========================
// =========================================================

void LogicSignal::Set(bool value)
{
   LogicOutput::SetOutputValue(value);
}

