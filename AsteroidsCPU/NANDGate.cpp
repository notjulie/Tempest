
#include "stdafx.h"
#include "NANDGate.h"

void NANDGate::AddSource(LogicOutput &source)
{
   sources.push_back(&source);
   source.OnValueChanged([this](bool newValue){
      UpdateValue();
   });
   UpdateValue();
}


void NANDGate::UpdateValue(void)
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

   SetOutputValue(!andResult);
}


