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


class iOSTempestEnvironment : public AbstractTempestEnvironment
{
public:
    virtual std::string GetDatabasePathName(void) { return ":memory:"; }
};

class iTempestSoundIO : public AbstractTempestSoundIO
{
public:
    virtual void SetSoundChannelState(int channel, SoundChannelState state) {};
    virtual void SetTime(uint64_t clockCycles) {};
    virtual uint8_t GetButtons(void) { return 0; };
    virtual uint8_t GetEncoder(void) {return 0;};
    virtual void SetButtonLED(ButtonFlag button, bool value) {};
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
    
private:
    iOSTempestEnvironment environment;
    iTempestSoundIO soundIO;
    TempestRunner runner;
};

#endif /* iTempest_hpp */
