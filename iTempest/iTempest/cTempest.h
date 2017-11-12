//
//  cTempest.h
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#ifndef cTempest_h
#define cTempest_h

#ifdef __cplusplus
    extern "C" {
#endif


typedef void *cTempest;

cTempest cTempestCreate(void);
void cTempestDispose(cTempest _tempest);


#ifdef __cplusplus
    };
#endif

        
#endif /* cTempest_h */
