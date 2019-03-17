
#ifndef VECTORMULTIGAME_H
#define VECTORMULTIGAME_H

#include "VectorGame.h"
class AbstractGameEnvironment;


class VectorMultiGame : public VectorGame
{
public:
   VectorMultiGame(AbstractGameEnvironment *_environment, std::initializer_list<std::function<VectorGame *()>> games);
   virtual ~VectorMultiGame(void);

   virtual CPU6502Runner *GetCPURunner(void);

   virtual void GetAllVectors(std::vector<DisplayVector> &vectors);
   virtual void SetControlPanel(AbstractArcadeGameControlPanelReader *controlPanel);
   virtual void SetSoundOutput(AbstractSoundGenerator *soundOutput);
   virtual void SingleStep(void);
   virtual void Start(void);

private:
   void DeleteGames(void);

private:
   AbstractGameEnvironment *environment;
   std::vector<VectorGame *> games;
   AbstractArcadeGameControlPanelReader *controlPanel = nullptr;
   int currentGameIndex = 0;
   bool menuButtonState = true;
   bool menuRequest = false;
};


#endif
