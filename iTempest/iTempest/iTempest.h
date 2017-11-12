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
#include "TempestRunner.h"


class iOSTempestEnvironment : public AbstractTempestEnvironment
{
public:
    virtual std::string GetDatabasePathName(void) { return ":memory:"; }
};


class iTempest
{
public:
    iTempest(void);
    
private:
    iOSTempestEnvironment environment;
    TempestRunner runner;
};

#endif /* iTempest_hpp */
