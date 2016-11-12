
#include "stdafx.h"
#include "ANDGate.h"

void ANDGate::AddSource(LogicOutput &source)
{
   sources.push_back(&source);
   source.OnValueChanged([this](bool newValue){
      UpdateValue();
   });
   UpdateValue();
}


void ANDGate::UpdateValue(void)
{
   bool andResult = true;
   for (auto source : sources)
   {
      if (!*source)
      {
         andResult = false;
         break;
      }
   }

   SetOutputValue(andResult);
}


