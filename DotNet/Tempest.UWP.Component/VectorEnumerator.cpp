
#include "pch.h"

#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"
#include "TempestIO/Vector/VectorData.h"

#include "VectorEnumerator.h"

namespace Tempest_UWP_Component {

   VectorEnumerator::VectorEnumerator(const std::vector<DisplayVector> &vectorList)
   {
      vectorData = new std::vector<DisplayVector>(vectorList);
      nextVector = 0;
   }

   VectorEnumerator::~VectorEnumerator(void)
   {
      delete vectorData, vectorData = NULL;
   }

   Vector VectorEnumerator::GetNextVector(void)
   {
      Vector result;
      result.valid = false;

      if (nextVector >= (int)vectorData->size())
         return result;

      DisplayVector	vector = (*vectorData)[nextVector++];
      if (vector.type != DisplayVector::Line)
         throw ref new Platform::NotImplementedException("VectorEnumerator::GetNextVector: unimplemented vector type");

      result.startX = vector.line.startX;
      result.startY = vector.line.startY;
      result.endX = vector.line.endX;
      result.endY = vector.line.endY;
      result.r = vector.line.r;
      result.g = vector.line.g;
      result.b = vector.line.b;

      result.valid = true;
      return result;
   }

}
