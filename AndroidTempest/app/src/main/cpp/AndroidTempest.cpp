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
/// Deletes an AndroidTempest given its handle
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

/// <summary>
/// Gets the current display vectors
/// </summary>
void AndroidTempest::GetVectors(int instance, std::vector<DisplayVector> &result)
{
   // clear
   result.resize(0);

   // get the instance
   auto ii = instances.find(instance);
   if (ii == instances.end())
      return;
   AndroidTempest *tempest = ii->second;

   // get the vectors
   tempest->game->GetAllVectors(result);
}



// ========================================================
//  JNI interface
// ========================================================

/// <summary>
/// creates an instance of AndroidTempest, returns an integer handle
/// <summary>
extern "C"
JNIEXPORT jint JNICALL
Java_com_notjulie_tempest_Tempest_createInstance(JNIEnv *, jclass) {
   return AndroidTempest::CreateInstance();
}

/// <summary>
/// deletes an instance of AndroidTempest, given its integer handle
/// <summary>
extern "C"
JNIEXPORT void JNICALL
Java_com_notjulie_tempest_Tempest_deleteInstance(JNIEnv *, jclass , jint instance) {
   AndroidTempest::DeleteInstance(instance);
}

/// <summary>
/// gets the latest screen vectors as a serialized byte array; each vector
/// occupies 11 bytes (int values are little-endian):
///   int16_t startX;
///   int16_t startY;
///   int16_t endX;
///   int16_t endY;
///   uint8_t r, g, b;
/// <summary>
extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_notjulie_tempest_Tempest_getVectors(JNIEnv *env, jclass , jint instance) {
   // get the instance's vectors
   std::vector<DisplayVector> vectors;
   AndroidTempest::GetVectors(instance, vectors);

   // serialize
   std::vector<uint8_t> serialized;
   for (auto v : vectors)
   {
      serialized.push_back(v.line.startX & 0xFF);
      serialized.push_back(v.line.startX >> 8);
      serialized.push_back(v.line.startY & 0xFF);
      serialized.push_back(v.line.startY >> 8);
      serialized.push_back(v.line.endX & 0xFF);
      serialized.push_back(v.line.endX >> 8);
      serialized.push_back(v.line.endY & 0xFF);
      serialized.push_back(v.line.endY >> 8);
      serialized.push_back(v.line.r);
      serialized.push_back(v.line.g);
      serialized.push_back(v.line.b);
   }

   // create, fill and return the result
   jbyteArray result = env->NewByteArray(11 * vectors.size());
   env->SetByteArrayRegion(result, 0, serialized.size(), (const jbyte *)&serialized[0]);
   return result;
}


