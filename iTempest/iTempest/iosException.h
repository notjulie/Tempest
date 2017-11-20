//
//  iosException.h
//  iTempest
//
//  Created by Randy Rasmussen on 11/19/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#ifndef iosException_h
#define iosException_h

#include <MacTypes.h>
#include "TempestException.h"

class iosException : public TempestException
{
public:
    iosException(const char *context, OSStatus status);
    
private:
    static std::string FormatMessage(const char *context, OSStatus status);
};

#endif /* iosException_h */
