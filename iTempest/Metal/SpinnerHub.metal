//
//  SpinnerHub.metal
//  iTempest
//
//  Created by Randy Rasmussen on 3/13/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#include <metal_math>
#include <metal_stdlib>
#include "RectangleShader.h"
#include "Spinner.h"
using namespace metal;


fragment half4 spinnerHubFragment(
                      RectangleShaderStageIn vertexInfo [[stage_in]],
                      const device SpinnerRenderParameters *vertexRenderParameters [[ buffer(SPINNER_RENDER_PARAMETERS_BUFFER) ]]
                      )
{
   float x = vertexInfo.xy[0];
   float y = vertexInfo.xy[1];

   // never mind if y is not visible
   if (y < -SPINNER_HEIGHT/2)
   {
      discard_fragment();
      return half4();
   }

   // if x is wider than the circle forget it
   if (abs(x) > SPINNER_RADIUS*metal::fast::sin(VISIBLE_DEGREES / 2 * M_PI_F / 180))
   {
      discard_fragment();
      return half4();
   }
   
   // find the y boundary of the circle for this X
   float circleY = metal::fast::sqrt(SPINNER_RADIUS*SPINNER_RADIUS - x*x);

   // subtract off how deeply the spinner is buried
   circleY -= SPINNER_RADIUS * metal::fast::cos(VISIBLE_DEGREES / 2 * M_PI_F / 180);
   
   // if it's inside the circle...
   if (y <= circleY - SPINNER_HEIGHT/2)
   {
      // calculate the angle of the point
      float angle = metal::fast::atan2(y + SPINNER_HEIGHT/2, x) * 180 / M_PI_F + vertexRenderParameters->rotation;
      while (angle < 0) { angle += 180; }
      while (angle > 180) { angle -= 180; }

      // my logic is not ideally mathematical right now because I'm not accounting properly for scaling factors,
      // but it gives a cool efffect so I'm happy with it... turn this into sort of a triangle wave thing centered
      // at 90 degrees
      float brightness = abs(angle - 90) / 90.0;
      return
         half4(
               0.125 + brightness/8,
               0.125 + brightness/8,
               0.125 + brightness/8,
               1.0
               );
   }
   
   // if it's straight above the circle...
   if (y <= circleY + SPINNER_HEIGHT/2)
   {
      // just return a background color for the surface of the wheel
      return
      half4(
            0.3,
            0.3,
            0.3,
            1.0
            );
   }
   
   // else never mind
   discard_fragment();
   return half4();
}

