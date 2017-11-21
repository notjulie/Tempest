//
//  iTempest.h
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#ifndef iTempest_h
#define iTempest_h

#include "AbstractTempestEnvironment.h"
#include "SimpleVector.h"
#include "TempestRunner.h"

#include "iTempestSoundIO.h"


class iOSTempestEnvironment : public AbstractTempestEnvironment
{
public:
    virtual std::string GetDatabasePathName(void) { return ":memory:"; }
};

class iTempestVectors
{
public:
    bool GetNext(
        int16_t *startX,
        int16_t *startY,
        int16_t *endX,
        int16_t *endY,
        uint8_t *r,
        uint8_t *g,
        uint8_t *b
    );

public:
    std::vector<SimpleVector>   vectors;
    
private:
    int index = -1;
};

class iTempest
{
public:
    iTempest(void);
    
    iTempestVectors *GetVectors(void);
    void SetPlayer1ButtonState(bool state) { soundIO.SetPlayer1ButtonState(state); }
    void SetFireButtonState(bool state) { soundIO.SetFireButtonState(state); }
    void SetZapButtonState(bool state) { soundIO.SetZapButtonState(state); }

private:
    iOSTempestEnvironment environment;
    iTempestSoundIO soundIO;
    TempestRunner runner;
};

#endif /* iTempest_hpp */
