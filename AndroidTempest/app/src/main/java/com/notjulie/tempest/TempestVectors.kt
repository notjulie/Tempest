package com.notjulie.tempest

class TempestVectors (var data: ByteArray) {
   var index = -1

   fun moveNext() : Boolean {
      if (index < 0) {
         index = 0
         return true
      }

      index += 11
      return index < data.size
   }

   fun getRed() : Byte { return data[index + 8] }
}
