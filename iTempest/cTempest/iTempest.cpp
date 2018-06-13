//
//  iTempest.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#include "iTempest.h"

iTempest::iTempest(void)
{
   game = new TempestGame(&environment);
   game->SetSoundOutput(&soundOutput);
   game->SetControlPanel(&controlPanel);

   runner = new VectorGameRunner(game);
   runner->Start();
}

iTempest::~iTempest(void)
{
   delete runner; runner = nullptr;
   delete game; game = nullptr;
}

int iTempest::GetVectors(TempestVector *buffer, int bufferSize)
{
   // get the latest screen image from the TempestRunner
   std::vector<SimpleVector> vectors;
   game->GetAllVectors(vectors);
   
   // figure out how many we're going to return
   int result = (int)vectors.size();
   if (result > bufferSize)
      result = bufferSize;
   
   // copy to the caller's buffer
   for (int i=0; i<result; ++i)
   {
      TempestVector *dest = buffer + i;
      SimpleVector *src = &vectors[i];
      dest->startX = src->startX;
      dest->startY = src->startY;
      dest->endX = src->endX;
      dest->endY = src->endY;
      dest->r = src->r;
      dest->g = src->g;
      dest->b = src->b;
   }
   
   // done
   return result;
}


