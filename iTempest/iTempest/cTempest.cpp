//
//  cTempest.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#include "iTempest.h"
#include "cTempest.h"

static std::mutex mutex;
static cTempest nextHandle = 1;
static std::map<cTempest, std::unique_ptr<iTempest> > tempestMap;

static iTempest *GetTempest(cTempest handle);

extern "C" {

    cTempest cTempestCreate(void)
    {
        std::lock_guard<std::mutex> lock(mutex);
        cTempest result = nextHandle++;
        tempestMap[result] = std::unique_ptr<iTempest>(new iTempest());
        return result;
    }
    
    void cTempestDispose(cTempest _tempest)
    {
        std::lock_guard<std::mutex> lock(mutex);
        tempestMap.erase(_tempest);
    }

    static iTempest *GetTempest(cTempest handle)
    {
        std::lock_guard<std::mutex> lock(mutex);
        return tempestMap[handle].get();
    }
    
    cVectors cTempestGetVectors(cTempest _tempest)
    {
        return GetTempest(_tempest)->GetVectors();
    }
    
    void cTempestSetButtonState(cTempest tempest, cTempestButton button, int state)
    {
        switch (button)
        {
            case FIRE:
                GetTempest(tempest)->SetFireButtonState(state);
                break;
            case PLAYER1:
                GetTempest(tempest)->SetPlayer1ButtonState(state);
                break;
            case PLAYER2:
                GetTempest(tempest)->SetPlayer2ButtonState(state);
                break;
            case ZAP:
                GetTempest(tempest)->SetZapButtonState(state);
                break;
        }
    }
    
    void cTempestMoveSpinner(cTempest _tempest, int offset)
    {
        return GetTempest(_tempest)->MoveSpinner(offset);
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

