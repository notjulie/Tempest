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

struct SpinnerVertex
{
    int position;
    int corner;
};

struct SpinnerRenderParameters
{
   float rotation;
};

#endif /* Spinner_h */
