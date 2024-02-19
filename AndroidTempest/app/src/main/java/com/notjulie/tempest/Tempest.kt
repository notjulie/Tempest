/**
 * Author: Randy Rasmussen
 * Copyright: 2024
 * Warranties: none
 *
 * Use for free as you will
 */

package com.notjulie.tempest

import android.os.Build
import androidx.annotation.RequiresApi
import java.lang.ref.Cleaner


/**
 * Kotlin wrapper around a native AndroidTempest instance
 */
@RequiresApi(Build.VERSION_CODES.TIRAMISU)
class Tempest {
   /**
    * our companions
    */
   companion object {
      @RequiresApi(Build.VERSION_CODES.TIRAMISU)
      val cleaner : Cleaner = Cleaner.create()
      @JvmStatic private external fun createInstance(): Int
      @JvmStatic private external fun deleteInstance(instance : Int)
      @JvmStatic private external fun getVectors(instance : Int): ByteArray
   }

   /**
    * handle to our native tempest object
    */
   var tempest: Int = createInstance()

   /**
    * initializer
    */
   init {
      // register ourself to be cleaned so that we can delete the
      // native object
      cleaner.register(this) { deleteInstance(tempest) }
   }

   public fun getCurrentVectors(): TempestVectors {
      return TempestVectors(getVectors(tempest))
   }
}