//
//  iTempest.h
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#ifndef iTempest_h
#define iTempest_h

// generic Tempest includes
#include "AbstractTempestEnvironment.h"
#include "SimpleVector.h"
#include "TempestRunner.h"

// iTempest includes
#include "iTempestSoundIO.h"
#include "TempestVector.h"


class iOSTempestEnvironment : public AbstractTempestEnvironment
{
public:
    virtual std::string GetDatabasePathName(void) { return ":memory:"; }
};

class iTempest
{
public:
    iTempest(void);
    
    int GetVectors(TempestVector *buffer, int bufferSize);
    void MoveSpinner(int offset) { soundIO.MoveSpinner(offset); }
    void SetPlayer1ButtonState(bool state) { soundIO.SetPlayer1ButtonState(state); }
    void SetPlayer2ButtonState(bool state) { soundIO.SetPlayer2ButtonState(state); }
    void SetFireButtonState(bool state) { soundIO.SetFireButtonState(state); }
    void SetZapButtonState(bool state) { soundIO.SetZapButtonState(state); }

private:
    iOSTempestEnvironment environment;
    iTempestSoundIO soundIO;
    TempestRunner runner;
};

#endif /* iTempest_hpp */
