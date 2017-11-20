//
//  iosException.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 11/19/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#include "iosException.h"

iosException::iosException(const char *context, OSStatus status)
    :
        TempestException(FormatMessage(context, status))
{
}

std::string iosException::FormatMessage(const char *context, OSStatus status)
{
    std::string result;
    
    result += context;
    result += ": ";
    
    switch (status)
    {
        case 1718449215:
            result += "invalid audio format";
            break;
            
        default:
            {
                char buf[100];
                sprintf(buf, "%d", (int)status);
                result += buf;
            }
            break;
    }
    
    return result;
}
