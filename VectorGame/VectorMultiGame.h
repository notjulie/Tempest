
#ifndef VECTORMULTIGAME_H
#define VECTORMULTIGAME_H

#include "VectorGame.h"

class VectorMultiGame : public VectorGame
{
public:
   VectorMultiGame(std::initializer_list<std::function<VectorGame *()>> games);
   virtual ~VectorMultiGame(void);

   virtual CPU6502Runner *GetCPURunner(void);

   virtual void GetAllVectors(std::vector<SimpleVector> &vectors);
   virtual void SetControlPanel(AbstractArcadeGameControlPanelReader *controlPanel);
   virtual void SetSoundOutput(AbstractTempestSoundOutput *soundOutput);
   virtual void SingleStep(void);
   virtual void Start(void);

private:
   void DeleteGames(void);

private:
   std::vector<VectorGame *> games;
   AbstractArcadeGameControlPanelReader *controlPanel = nullptr;
   int currentGameIndex = 0;
};


#endif
