//
//  iTempest.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 11/12/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#include "iTempest.h"

iTempest::iTempest(void)
    :
        runner(&environment)
{
    runner.SetTempestIO(&soundIO);
    runner.Start();
}

int iTempest::GetVectors(TempestVector *buffer, int bufferSize)
{
   // get the latest screen image from the TempestRunner
   std::vector<SimpleVector> vectors;
   runner.GetAllVectors(vectors);
   
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
      dest->r = src->color.GetR();
      dest->g = src->color.GetG();
      dest->b = src->color.GetB();
   }
   
   // done
   return result;
}


