//
//  RectangleShader.metal
//  iTempest
//
//  Created by Randy Rasmussen on 3/25/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#include <metal_stdlib>
#include "RectangleShader.h"
using namespace metal;


vertex RectangleShaderStageIn rectangleShaderVertex(
                                               const device RectangleShaderVertexParameters *vertexRenderParameters [[ buffer(RECTANGLE_SHADER_VERTEX_PARAMETERS_BUFFER_INDEX) ]],
                                               unsigned int vid [[ vertex_id ]])
{
   RectangleShaderStageIn result;
   
   switch (vid)
   {
      case 0:
         result.position = float4(
                                  vertexRenderParameters->centerX - vertexRenderParameters->xScale,
                                  vertexRenderParameters->centerY - vertexRenderParameters->yScale,
                                  0, 1.0
                                  );
         result.xy = float2(-1.0, -1.0);
         break;
         
      case 1:
         result.position =  float4(
                                   vertexRenderParameters->centerX - vertexRenderParameters->xScale,
                                   vertexRenderParameters->centerY + vertexRenderParameters->yScale,
                                   0, 1.0
                                   );
         result.xy = float2(-1.0, +1.0);
         break;
         
      case 2:
         result.position =  float4(
                                   vertexRenderParameters->centerX + vertexRenderParameters->xScale,
                                   vertexRenderParameters->centerY - vertexRenderParameters->yScale,
                                   0, 1.0
                                   );
         result.xy = float2(+1.0, -1.0);
         break;
         
      case 3:
      default:
         result.position =  float4(
                                   vertexRenderParameters->centerX + vertexRenderParameters->xScale,
                                   vertexRenderParameters->centerY + vertexRenderParameters->yScale,
                                   0, 1.0
                                   );
         result.xy = float2(+1.0, +1.0);
         break;
         
   }
   
   return result;
}


