//
//  Test.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 10/23/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#include <stdio.h>

#include "AbstractTempestEnvironment.h"
#include "TempestRunner.h"

class iOSTempestEnvironment : public AbstractTempestEnvironment
{
public:
    virtual void Reset(void) {}
    virtual void Sleep(int ms) {}
    virtual void SynchronizeClock(uint64_t busMSCount) {}
    virtual std::string GetDatabasePathName(void) { return "bleem"; }
};

void Test()
{
    iOSTempestEnvironment environment;
    TempestRunner runner(&environment);
}
