//
//  RectangleShader.h
//  iTempest
//
//  Created by Randy Rasmussen on 3/25/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#ifndef RectangleShader_h
#define RectangleShader_h

#define RECTANGLE_SHADER_VERTEX_PARAMETERS_BUFFER_INDEX 0

/**
 Parameters passed to the RectangleShader vertex routine
 */
struct RectangleShaderVertexParameters
{
   // center X and Y position in the -1...+1 coordinates of Metal
   float centerX, centerY;
   
   // relative width and height in the -1...+1 coordinates of Metal
   float xScale, yScale;
};


// ====================================================
// ============ Metal-specific types ==================
// ====================================================

#ifdef __METAL_VERSION__

/**
 The stage_in value passed to a rectangle shader
 */
struct RectangleShaderStageIn
{
   // position as calculated by the vertex function
   metal::float4 position [[position]];
   
   // relative position within the rectangle (in -1..+1 range)
   metal::float2 xy;
};


#endif

#endif /* RectangleShader_h */
