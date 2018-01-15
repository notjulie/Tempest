//
//  TempestLayer.metal
//  iTempest
//
//  Created by Randy Rasmussen on 1/13/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#include <metal_stdlib>
#include "TempestLayer.h"
using namespace metal;

// these are defined in the source coordinate system, which is a 65536x65536 square
#define DOT_SIZE 200.0
#define LINE_WIDTH 200.0

vertex float4 tempestVertex(
                           device TempestVector* vectorArray [[ buffer(0) ]],
                           unsigned int vid [[ vertex_id ]])
{
   // we have 6 vertices per vector = two triangles
   TempestVector v = vectorArray[vid / 6];
   
   // if the start and end points are the same this is a dot
   if (v.startX==v.endX && v.startY==v.endY)
   {
      switch (vid % 6)
      {
         case 0:
            return float4((v.startX - DOT_SIZE/2) / 32768.0, (v.startY) / 32768.0, 0, 1.0);
         case 1:
         case 3:
            return float4((v.startX) / 32768.0, (v.startY + DOT_SIZE) / 32768.0, 0, 1.0);
         case 2:
         case 4:
            return float4((v.startX - DOT_SIZE) / 32768.0, (v.startY) / 32768.0, 0, 1.0);
         case 5:
         default:
            return float4((v.startX) / 32768.0, (v.startY - DOT_SIZE) / 32768.0, 0, 1.0);
      }
   }
   
   // calculate the vector that gives us our line width
   float lineLength = length(float2(v.endX - v.startX, v.endY - v.startY));
   float normalizedDX = LINE_WIDTH * (v.endX - v.startX) / lineLength / 2;
   float normalizedDY = LINE_WIDTH * (v.endY - v.startY) / lineLength / 2;

   switch (vid % 6)
   {
      case 0:
         return float4((v.startX - normalizedDY) / 32768.0, (v.startY + normalizedDX) / 32768.0, 0, 1.0);
      case 1:
      case 3:
         return float4((v.startX + normalizedDY) / 32768.0, (v.startY - normalizedDX) / 32768.0, 0, 1.0);
      case 2:
      case 4:
         return float4((v.endX - normalizedDY) / 32768.0, (v.endY + normalizedDX) / 32768.0, 0, 1.0);
      case 5:
      default:
         return float4((v.endX + normalizedDY) / 32768.0, (v.endY - normalizedDX) / 32768.0, 0, 1.0);
   }
}

fragment half4 tempestFragment(float2 pointCoord  [[point_coord]])
{
   //if (length(pointCoord - float2(0.5)) > 0.5) {
   //    discard_fragment();
   //}
   return half4(1.0, 1.0, 1.0, 1.0 - 4.0*length(pointCoord - float2(0.5)));
}

