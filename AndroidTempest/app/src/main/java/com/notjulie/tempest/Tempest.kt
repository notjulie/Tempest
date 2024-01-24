/**
 * Author: Randy Rasmussen
 * Copyright: 2024
 * Warantees: none
 *
 * Use for free as you will
 */

package com.notjulie.tempest

import java.lang.ref.Cleaner


/**
 * Kotlin wrapper around a native AndroidTempest instance
 */
class Tempest {
   /**
    * our companions
    */
   companion object {
      val cleaner : Cleaner = Cleaner.create()
      @JvmStatic private external fun createInstance(): Int
      @JvmStatic private external fun deleteInstance(instance : Int)
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

}