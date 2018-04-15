//
//  PlayerButtonRenderer.metal
//  iTempest
//
//  Created by Randy Rasmussen on 3/25/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#include <metal_math>
#include <metal_stdlib>
#include "PlayerButtonRenderer.h"
#include "RectangleShader.h"
using namespace metal;


fragment half4 playerButtonShader(
                   RectangleShaderStageIn vertexInfo [[stage_in]],
                   const device PlayerButtonSettings *settings [[ buffer(PLAYER_BUTTON_SETTINGS_BUFFER_INDEX) ]]
                   )
{
   float x = vertexInfo.xy[0];
   float y = vertexInfo.xy[1];
   
   // calculate the height of our ellipse as a function of x
   float ellipseHeight = 0.8 * metal::fast::sqrt(1 - x*x);
   
   // skip points above the surface
   if (y > 0.2 + ellipseHeight) {
      discard_fragment();
      return half4();
   }

   // return points on the surface
   if (y > 0.2 - ellipseHeight) {
      return half4(settings->r, settings->g, settings->b, 1.0);
   }

   // return points on the edge
   if (y > -0.2 - ellipseHeight) {
      return half4(settings->r * 0.5, settings->g*0.5, settings->b*0.5, 1.0);
   }

   // skip the rest
   discard_fragment();
   return half4();
}

