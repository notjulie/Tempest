//
//  Spinner.metal
//  iTempest
//
//  Created by Randy Rasmussen on 1/7/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#include <metal_math>
#include <metal_stdlib>
#include "Spinner.h"
using namespace metal;

#define VISIBLE_DEGREES 60.0
#define SPINNER_RADIUS 1.0
#define SPINNER_HEIGHT 0.3

vertex float4 spinnerVertex(
      const device SpinnerVertex* vertex_array [[ buffer(SPINNER_VERTICES_BUFFER) ]],
      const device SpinnerRenderParameters *vertexRenderParameters [[ buffer(SPINNER_RENDER_PARAMETERS_BUFFER) ]],
      unsigned int vid [[ vertex_id ]])
{
   // calculate the rotation angle of this vector
   float angle = (vertex_array[vid/6].position * VISIBLE_DEGREES) + vertexRenderParameters->rotation;

   // do a modulus of the rotation angle so that our vectors wrap around; make
   // sure that the angle is positive regardless of any quirks of integer math
   float wrapAroundCount = floor(angle / VISIBLE_DEGREES);
   angle -= wrapAroundCount * VISIBLE_DEGREES;
   
   // and center
   angle -= VISIBLE_DEGREES / 2;

   // get sin/cosine
   float radians = angle * M_PI_F / 180;
   float angleCos = metal::fast::cos(radians);
   float angleSin = metal::fast::sin(radians);
   
   // calculate the location of the line
   float y = SPINNER_RADIUS * (angleCos - cos(M_PI_F * VISIBLE_DEGREES / 2 / 180));
   float x = SPINNER_RADIUS * angleSin;

   // we get called 6 times for each vector so that we can return the vertices of two triangles
   float4 result;
   switch (vid % 6)
   {
      case 0:
         result = float4(x - 0.01, y - SPINNER_HEIGHT/2, 0, 1.0);
         break;

      case 1:
      case 3:
         result = float4(x + 0.01, y - SPINNER_HEIGHT/2, 0, 1.0);
         break;

      case 2:
      case 4:
         result = float4(x - 0.01, y + SPINNER_HEIGHT, 0, 1.0);
         break;

      case 5:
      default:
         result = float4(x + 0.01, y + SPINNER_HEIGHT, 0, 1.0);
         break;
   }
   
   result[0] = vertexRenderParameters->centerX + result[0] * vertexRenderParameters->xScale;
   result[1] = vertexRenderParameters->centerY + result[1] * vertexRenderParameters->yScale;
   return result;
}

fragment half4 spinnerFragment(float4 pointCoord  [[position]])
{
    return half4(0.7, 0.7, 0.7, 1.0);
}

