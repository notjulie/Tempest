//
//  cTempest.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#include "iTempest.h"
#include "cTempest.h"

static std::mutex mutex;
static cTempest nextHandle = 1;
static std::map<cTempest, std::unique_ptr<iTempest> > tempestMap;

static iTempest *GetTempest(cTempest handle);

extern "C" {

   cTempest cTempestCreate(void)
   {
     std::lock_guard<std::mutex> lock(mutex);
     cTempest result = nextHandle++;
     tempestMap[result] = std::unique_ptr<iTempest>(new iTempest());
     return result;
   }

   void cTempestDispose(cTempest _tempest)
   {
     std::lock_guard<std::mutex> lock(mutex);
     tempestMap.erase(_tempest);
   }

   static iTempest *GetTempest(cTempest handle)
   {
     std::lock_guard<std::mutex> lock(mutex);
     return tempestMap[handle].get();
   }

   int cTempestGetVectors(cTempest tempest, TempestVector *buffer, int bufferSize)
   {
      return GetTempest(tempest)->GetVectors(buffer, bufferSize);
   }
    
   void cTempestSetButtonState(cTempest tempest, cTempestButton button, int state)
   {
     switch (button)
     {
         case FIRE:
             GetTempest(tempest)->SetFireButtonState(state);
             break;
         case PLAYER1:
             GetTempest(tempest)->SetPlayer1ButtonState(state);
             break;
         case PLAYER2:
             GetTempest(tempest)->SetPlayer2ButtonState(state);
             break;
         case ZAP:
             GetTempest(tempest)->SetZapButtonState(state);
             break;
     }
   }

   void cTempestMoveSpinner(cTempest _tempest, int offset)
   {
     return GetTempest(_tempest)->MoveSpinner(offset);
   }

   int cTempestIsButtonLit(cTempest tempest, cTempestButton button)
   {
      switch (button)
      {
         case PLAYER1:
            return GetTempest(tempest)->GetPlayer1LEDState() ? 1 : 0;
         case PLAYER2:
            return GetTempest(tempest)->GetPlayer2LEDState() ? 1 : 0;
            
         case FIRE:
         case ZAP:
            // "lit" is "visible"... they are always visible
            return 1;
      }

      return 0;
   }
   
   int cTempestIsInAttractMode(cTempest tempest)
   {
      return GetTempest(tempest)->IsInAttractMode() ? 1 : 0;
   }
};

