//
//  TempestLayer.metal
//  iTempest
//
//  Created by Randy Rasmussen on 1/13/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#include <metal_stdlib>
#include "cTempest/TempestVector.h"
using namespace metal;

// these are defined in the source coordinate system, which is a 65536x65536 square
#define DOT_SIZE 200.0
#define LINE_WIDTH 200.0

struct VertexResult
{
   float4 position [[position]];
   half4 color;
};


float4 calculateVertexPosition(TempestVector v, int vertexIndex)
{
   // if the start and end points are the same this is a dot
   if (v.startX==v.endX && v.startY==v.endY)
   {
      switch (vertexIndex % 6)
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
   
   switch (vertexIndex % 6)
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

vertex VertexResult tempestVertex(
                           device TempestVector* vectorArray [[ buffer(TEMPEST_VERTICES_BUFFER) ]],
                           device TempestRenderParameters* renderParameters [[ buffer(TEMPEST_RENDER_PARAMETERS_BUFFER) ]],
                           unsigned int vid [[ vertex_id ]])
{
   // we have 6 vertices per vector = two triangles
   TempestVector v = vectorArray[vid / 6];

   VertexResult result;
   result.position = calculateVertexPosition(v, vid % 6);
   result.position[0] = renderParameters->centerX + result.position[0] * renderParameters->xScale;
   result.position[1] = renderParameters->centerY + result.position[1] * renderParameters->yScale;
   result.color = half4(v.r / 255.0, v.g/255.0, v.b/255.0, 1.0);
   return result;
}

fragment half4 tempestFragment(VertexResult vertexInfo [[stage_in]])
{
   return vertexInfo.color;
}

