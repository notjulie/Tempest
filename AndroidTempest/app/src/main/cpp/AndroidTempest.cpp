//
// Created by Randy Rasmussen on 1/14/24.
//

#include "AndroidTempest.h"
#include "TempestException.h"

AndroidTempest::AndroidTempest(void)
{
    game = new TempestGame(&environment);
    game->SetSoundOutput(&soundOutput);
    game->SetControlPanel(&controlPanel);

    runner = new VectorGameRunner(game);
    runner->Start();
}

AndroidTempest::~AndroidTempest(void)
{
    delete runner; runner = nullptr;
    delete game; game = nullptr;
}

/*int AndroidTempest::GetVectors(TempestVector *buffer, int bufferSize)
{
    // get the latest screen image from the TempestRunner
    std::vector<DisplayVector> vectors;
    game->GetAllVectors(vectors);

    // figure out how many we're going to return
    int result = (int)vectors.size();
    if (result > bufferSize)
        result = bufferSize;

    // copy to the caller's buffer
    for (int i=0; i<result; ++i)
    {
        TempestVector *dest = buffer + i;
        DisplayVector *src = &vectors[i];
        if (src->type != DisplayVector::Line)
            throw TempestException("iTempest::GetVectors: unsupported vector type");
        dest->startX = src->line.startX;
        dest->startY = src->line.startY;
        dest->endX = src->line.endX;
        dest->endY = src->line.endY;
        dest->r = src->line.r;
        dest->g = src->line.g;
        dest->b = src->line.b;
    }

    // done
    return result;
}*/

