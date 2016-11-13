
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
   bool orResult = true;
   for (auto source : sources)
   {
      if (*source)
      {
         orResult = true;
         break;
      }
   }

   SetOutputValue(orResult);
}


