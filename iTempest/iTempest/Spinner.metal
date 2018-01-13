//
//  Spinner.metal
//  iTempest
//
//  Created by Randy Rasmussen on 1/7/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#include <metal_stdlib>
#include "Spinner.h"
using namespace metal;

typedef struct
{
    // Positions in pixel space
    // (e.g. a value of 100 indicates 100 pixels from the center)
    vector_float2 position;
    
    // Floating-point RGBA colors
    vector_float4 color;
} AAPLVertex;

vertex float4 basic_vertex(
        const device SpinnerVertex* vertex_array [[ buffer(0) ]],
        unsigned int vid [[ vertex_id ]])
{
    if (vertex_array[vid].corner == 0)
        return float4(0.0, 1.0, 0.0, 1.0);
    if (vertex_array[vid].corner == 1)
        return float4(-1.0, -1.0, 0, 1.0);
    return float4(1.0, -1.0, 0, 1.0);
}

fragment half4 basic_fragment(float4 pointCoord  [[position]])
{
//    if (length(pointCoord - float2(0.5)) > 0.5) {
//        discard_fragment();
//    }
    return half4(pointCoord.x / 100);
}

