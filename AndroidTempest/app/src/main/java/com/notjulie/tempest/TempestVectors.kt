package com.notjulie.tempest


/**
 * enumerator for Tempest display vectors; we are a wrapper around a
 * serialized byte array that consists of an array of elements whose
 * C transliteration would be:
 *
 * serialize; the C struct transliteration of this would be:
 * struct {
 *    uint8_t startXlo;
 *    uint8_t startXhi;
 *    uint8_t startYlo;
 *    uint8_t startYhi;
 *    uint8_t endXlo;
 *    uint8_t endXhi;
 *    uint8_t endYlo;
 *    uint8_t endYhi;
 *    uint8_t r,g,b;
 * };
 */
class TempestVectors (var data: ByteArray) {
   private var index = -1

   /**
    * moves to the next vector in the list; returns false if we were
    * already at the end of the list
    */
   fun moveNext() : Boolean {
      if (index < 0) {
         index = 0
      } else if (index < data.size) {
         index += 11
      }

      return index < data.size
   }

   fun getColor() : Int {
      val r : Int = 0xFF and data[index + 8].toInt()
      val g : Int = 0xFF and data[index + 9].toInt()
      val b : Int = 0xFF and data[index + 10].toInt()
      return (r shl 16) or (g shl 8) or b
   }

   fun getStartX() : Int {
      return getInt16(index + 0)
   }

   fun getStartY() : Int {
      return getInt16(index + 2)
   }

   fun getEndX() : Int {
      return getInt16(index + 4)
   }

   fun getEndY() : Int {
      return getInt16(index + 6)
   }

   private fun getInt16(offset: Int) : Int {
      val lo : Int = 0xFF and data[offset].toInt()
      val hi : Int = data[offset + 1].toInt()
      return (hi shl 8) or lo
   }
}
