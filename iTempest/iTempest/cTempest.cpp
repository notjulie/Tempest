//
//  cTempest.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#include "iTempest.h"
#include "cTempest.h"

extern "C" {

    cTempest cTempestCreate(void)
    {
        return new iTempest();
    }

    void cTempestDispose(cTempest _tempest)
    {
        delete (iTempest *)_tempest;
    }

    cVectors cTempestGetVectors(cTempest _tempest)
    {
        return ((iTempest *)_tempest)->GetVectors();
    }
    
    void cVectorsDispose(cVectors _vectors)
    {
        delete (iTempestVectors *)_vectors;
    }
    
    int cVectorsGetNext(
                        cVectors _vectors,
                        int16_t *startX,
                        int16_t *startY,
                        int16_t *endX,
                        int16_t *endY,
                        uint8_t *r,
                        uint8_t *g,
                        uint8_t *b
                        )
    {
        iTempestVectors *vectors = (iTempestVectors *)_vectors;
        bool result = vectors->GetNext(
                startX, startY,
                endX, endY,
                r, g, b);
        return result ? 1 : 0;
    }

    
};

