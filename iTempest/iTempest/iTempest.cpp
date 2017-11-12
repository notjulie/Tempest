//
//  iTempest.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#include "iTempest.h"

iTempest::iTempest(void)
    :
        runner(&environment)
{
    runner.SetTempestIO(&soundIO);
    runner.Start();
}

iTempestVectors *iTempest::GetVectors(void)
{
    iTempestVectors *result = new iTempestVectors();
    runner.GetAllVectors(result->vectors);
    return result;
}

bool iTempestVectors::GetNext(
             int16_t *startX,
             int16_t *startY,
             int16_t *endX,
             int16_t *endY,
             uint8_t *r,
             uint8_t *g,
             uint8_t *b
             )
{
    if (++index < vectors.size())
    {
        *startX = vectors[index].startX;
        *startY = vectors[index].startY;
        *endX = vectors[index].endX;
        *endY = vectors[index].endY;
        
        TempestColor color = vectors[index].color;
        *r = color.GetR();
        *g = color.GetG();
        *b = color.GetB();
        return true;
    }
    
    return false;
}
