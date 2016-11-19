
#include "stdafx.h"
#include "ORGate.h"


void ORGate::AddSource(LogicOutput &source)
{
   sources.push_back(&source);
   source.OnValueChanged([this](bool newValue){
      UpdateValue();
   });
   UpdateValue();
}


void ORGate::UpdateValue(void)
{
   bool orResult = false;
   for (auto source : sources)
   {
      if (*source)
      {
         orResult = true;
         break;
      }
   }

   if (name == "vsmROMLatchClockSource")
      int i = 1;
   SetOutputValue(orResult);
}


