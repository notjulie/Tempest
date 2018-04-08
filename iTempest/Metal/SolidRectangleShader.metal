//
//  SolidRectangleShader.metal
//  iTempest
//
//  Created by Randy Rasmussen on 4/8/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#include <metal_stdlib>
#include "RectangleShader.h"
#include "SolidRectangleShader.h"
using namespace metal;


fragment half4 solidRectangleFragmentFunction(
                                  RectangleShaderStageIn vertexInfo [[stage_in]],
                                  const device SolidRectangleRenderParameters *renderParameters [[ buffer(SOLID_RECTANGLE_RENDER_PARAMETERS_BUFFER) ]]
                                  )
{
   if (renderParameters->a == 0)
      discard_fragment();
   return half4(renderParameters->r, renderParameters->g, renderParameters->b, renderParameters->a);
}

