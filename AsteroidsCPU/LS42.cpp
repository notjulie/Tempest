
#include "stdafx.h"
#include "LS42.h"


LS42::LS42(void)
{
   a.OnValueChanged([this](bool newValue){
      UpdateValue();
   });
   b.OnValueChanged([this](bool newValue){
      UpdateValue();
   });
   c.OnValueChanged([this](bool newValue){
      UpdateValue();
   });
   d.OnValueChanged([this](bool newValue){
      UpdateValue();
   });
}


void LS42::UpdateValue(void)
{
   uint8_t selectedOutput = GetInput();
   if (selectedOutput > 9)
      selectedOutput = 9;
   for (int i = 0; i < 10; ++i)
      outputs[i].Set(i != selectedOutput);
}


uint8_t LS42::GetInput(void) const
{
   uint8_t selectedOutput = 0;
   if (a)
      selectedOutput += 1;
   if (b)
      selectedOutput += 2;
   if (c)
      selectedOutput += 4;
   if (d)
      selectedOutput += 8;
   return selectedOutput;
}
