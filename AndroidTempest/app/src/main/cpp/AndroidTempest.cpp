//
// Created by Randy Rasmussen on 1/14/24.
//

#include <jni.h>
#include "AndroidTempest.h"
#include "TempestException.h"


/// <summary>
/// Class static members
/// </summary>
int AndroidTempest::nextInstanceHandle = 1;
std::map<int, AndroidTempest*> AndroidTempest::instances;


/// <summary>
/// Initializes a new instance of class AndroidTempest
/// </summary>
AndroidTempest::AndroidTempest()
{
    game = new TempestGame(&environment);
    game->SetSoundOutput(&soundOutput);
    game->SetControlPanel(&controlPanel);

    runner = new VectorGameRunner(game);
    runner->Start();
}

/// <summary>
/// Releases resources held by the object
/// </summary>
AndroidTempest::~AndroidTempest()
{
    delete runner; runner = nullptr;
    delete game; game = nullptr;
}



// ========================================================
//  static methods for JNI interface
// ========================================================

/// <summary>
/// Creates an AndroidTempest instance and returns a numeric handle to it
/// </summary>
int AndroidTempest::CreateInstance()
{
   instances[nextInstanceHandle] = new AndroidTempest();
   return nextInstanceHandle++;
}

/// <summary>
/// Deletes an AnsdroidTempest given its handle
/// </summary>
void AndroidTempest::DeleteInstance(int instance)
{
   auto ii = instances.find(instance);
   if (ii != instances.end())
   {
      delete ii->second;
      instances.erase(ii);
   }
}


// ========================================================
//  JNI interface
// ========================================================

extern "C"
JNIEXPORT jint JNICALL
Java_com_notjulie_tempest_Tempest_createInstance(JNIEnv *env, jclass clazz) {
   return AndroidTempest::CreateInstance();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_notjulie_tempest_Tempest_deleteInstance(JNIEnv *env, jclass clazz, jint instance) {
   AndroidTempest::DeleteInstance(instance);
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





