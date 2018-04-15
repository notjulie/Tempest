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


/// <summary>
/// This is the vertex function used by RectangleShader objects... using the parameters
/// in the RectangleShaderVertexParameters it will cause the fragment function to be
/// called for every point in the specified rectangle.  The fragment function's stage in
/// will be of type RectangleShaderStageIn, which will contain an XY pair describing the
/// relative position of the point within the rectangle, with -1<x<1 and -1<y<1
/// </summary>
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


