//
//  cTempest.h
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#ifndef cTempest_h
#define cTempest_h

#include "TempestVector.h"

#ifdef __cplusplus
extern "C" {
#endif

   // a nice simple abstraction for passing around a reference to a tempest
   // object... this is basically just an integer handle that the C code maps
   // to an actual tempest object
   typedef int cTempest;

   typedef enum {
        FIRE,
        PLAYER1,
        PLAYER2,
        ZAP
   } cTempestButton;
    
   cTempest cTempestCreate(void);
   cTempest cTempestNull(void);
   void cTempestDispose(cTempest _tempest);
   int cTempestGetVectors(cTempest tempest, TempestVector *buffer, int bufferSize);
   void cTempestMoveSpinner(cTempest _tempest, int offset);
   void cTempestSetButtonState(cTempest tempest, cTempestButton button, int state);
   int cTempestIsButtonLit(cTempest tempest, cTempestButton button);
   
#ifdef __cplusplus
};
#endif

        
#endif /* cTempest_h */
