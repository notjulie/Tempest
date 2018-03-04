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
   
#ifdef __cplusplus
};
#endif

        
#endif /* cTempest_h */
