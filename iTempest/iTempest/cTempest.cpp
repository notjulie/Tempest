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

};

