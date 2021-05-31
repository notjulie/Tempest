
#ifndef VECTORENUMERATOR_H
#define VECTORENUMERATOR_H

#include <vector>
using namespace Platform;
//using namespace System::Runtime::InteropServices;

#include "DisplayVector.h"

class SimpleVectorDataInterpreter;
class VectorData;

namespace Tempest_UWP_Component {

   public value struct Vector
   {
   public:
      bool valid;
      int16_t startX;
      int16_t startY;
      int16_t endX;
      int16_t endY;
      uint8_t r;
      uint8_t g;
      uint8_t b;
   };

   public ref class VectorEnumerator sealed
   {
   internal:
      VectorEnumerator(const std::vector<DisplayVector> &vectorList);

   public:
      virtual ~VectorEnumerator(void);

      Vector GetNextVector(void);

   private:
      std::vector<DisplayVector> *vectorData;
      int nextVector;
   };

}

#endif
