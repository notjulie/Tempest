//
//  Spinner.h
//  iTempest
//
//  Created by Randy Rasmussen on 1/13/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

#ifndef Spinner_h
#define Spinner_h

#include <simd/SIMD.h>

#define SPINNER_VERTICES_BUFFER 0
#define SPINNER_RENDER_PARAMETERS_BUFFER 1

#define VISIBLE_DEGREES 150.0
#define SPINNER_RADIUS 0.6
#define SPINNER_HEIGHT 0.6

struct SpinnerVertex
{
    float position;
};

struct SpinnerRenderParameters
{
   float rotation;
   float centerX, centerY;
   float xScale, yScale;
};

#endif /* Spinner_h */
