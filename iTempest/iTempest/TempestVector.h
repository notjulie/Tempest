//
//  TempestVector.h
//  iTempest
//
//  Created by Randy Rasmussen on 1/13/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//
//  This structure is used by Metal, Swift and C, so it's just simplest
//  to devote a file to it.
//

#ifndef TempestVector_h
#define TempestVector_h

#ifdef __cplusplus
   extern "C" {
#endif

#define TEMPEST_VERTICES_BUFFER 0
#define TEMPEST_RENDER_PARAMETERS_BUFFER 1

//
// A structure that defines a screen vector in the way that our Metal
// functions expect it to be presented.  Everyone else should cooperate
// with Metal and use this.
//
typedef struct _TempestVector
{
   short startX;
   short startY;
   short endX;
   short endY;
   unsigned char  r;
   unsigned char  g;
   unsigned char  b;
} TempestVector;

typedef struct _TempestRenderParameters
{
   float centerX, centerY;
   float xScale, yScale;
} TempestRenderParameters;
      
#ifdef __cplusplus
   };
#endif

#endif /* TempestVector_h */
