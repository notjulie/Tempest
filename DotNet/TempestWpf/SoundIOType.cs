using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TempestWpf
{
   /// <summary>
   /// The type of connection between the game and the sound/control panel objects
   /// </summary>
   public enum SoundIOType
   {
      /// <summary>
      /// Direct object connection between control panel/sound and game processor
      /// </summary>
      Direct,

      /// <summary>
      /// COM port connection to Discovery board
      /// </summary>
      Discovery,

      /// <summary>
      /// Serial connection using loopback between two COM ports
      /// </summary>
      Loopback,

      /// <summary>
      /// Serial connection between control panel/sound and game processor via a
      /// memory stream
      /// </summary>
      MemoryStream
   }
}
