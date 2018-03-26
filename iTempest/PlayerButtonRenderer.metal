//
//  PlayerButtonRenderer.metal
//  iTempest
//
//  Created by Randy Rasmussen on 3/25/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#include <metal_math>
#include <metal_stdlib>
#include "RectangleShader.h"
using namespace metal;


fragment half4 playerButtonShader(
                   RectangleShaderStageIn vertexInfo [[stage_in]]
                   )
{
   float x = vertexInfo.xy[0];
   float y = vertexInfo.xy[1];
   
   if (x*x + y*y <= 1.0) {
      return half4(1.0, 0.0, 0.0, 1.0);
   } else {
      discard_fragment();
      return half4();
   }
}

