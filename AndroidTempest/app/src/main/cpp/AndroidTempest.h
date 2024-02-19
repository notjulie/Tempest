//
// Created by Randy Rasmussen on 1/14/24.
//

#ifndef ANDROIDTEMPEST_ANDROIDTEMPEST_H
#define ANDROIDTEMPEST_ANDROIDTEMPEST_H


// generic Tempest includes
#include "AbstractGameEnvironment.h"
#include "DisplayVector.h"
#include "VectorGameRunner.h"
#include "TempestGame.h"

// AndroidTempest includes
#include "AndroidTempestSoundOutput.h"


class AndroidTempest final
{
private:
   // we only allow instantiation/destruction via our static methods
   AndroidTempest();
   ~AndroidTempest();

public:
   void GetVectors(std::vector<DisplayVector> &result);
   bool IsInAttractMode() { return game->IsInAttractMode(); }

   // dispatchers to the control panel
   bool GetPlayer1LEDState() { return controlPanel.GetButtonLED(ONE_PLAYER_BUTTON); }
   bool GetPlayer2LEDState() { return controlPanel.GetButtonLED(TWO_PLAYER_BUTTON); }
   void MoveSpinner(int offset) { controlPanel.MoveEncoder(offset); }
   void SetPlayer1ButtonState(bool state) { controlPanel.SetButtonState(ONE_PLAYER_BUTTON, state); }
   void SetPlayer2ButtonState(bool state) { controlPanel.SetButtonState(TWO_PLAYER_BUTTON, state); }
   void SetFireButtonState(bool state) { controlPanel.SetButtonState(FIRE_BUTTON, state); }
   void SetZapButtonState(bool state) { controlPanel.SetButtonState(ZAPPER_BUTTON, state); }

public:
   // static interface for JNI calls
   static int CreateInstance();
   static void DeleteInstance(int instance);
   static AndroidTempest *GetInstance(int instanceHandle);

private:
   static int nextInstanceHandle;
   static std::map<int, AndroidTempest*> instances;



private:
   AbstractGameEnvironment environment;
   AndroidTempestSoundOutput soundOutput;
   VectorGameRunner *runner = nullptr;
   TempestGame *game = nullptr;
   SimpleArcadeGameControlPanel controlPanel;
};


#endif //ANDROIDTEMPEST_ANDROIDTEMPEST_H
