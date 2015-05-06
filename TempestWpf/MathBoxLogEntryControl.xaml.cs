using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TempestWpf
{
   /// <summary>
   /// Interaction logic for MathBoxLogEntryControl.xaml
   /// </summary>
   public partial class MathBoxLogEntryControl : UserControl
   {
      private MathBoxLogEntry entry;
      private int recordNumber;

      /// <summary>
      /// Initializes a new instance of class MathBoxLogEntryControl
      /// </summary>
      public MathBoxLogEntryControl()
      {
         InitializeComponent();
      }

      /// <summary>
      /// Gets or sets the entry we're displaying
      /// </summary>
      public MathBoxLogEntry Entry
      {
         get
         {
            return entry;
         }
         set
         {
            entry = value;
            UpdateControls();
         }
      }

      /// <summary>
      /// Gets or sets the record number
      /// </summary>
      public int RecordNumber
      {
         get
         {
            return this.recordNumber;
         }
         set
         {
            this.recordNumber = value;
            UpdateControls();
         }
      }

      private void UpdateControls()
      {
         // special case if our entry is null
         MathBoxLogEntry entry = this.entry;
         if (entry == null)
         {
            recordNumberLabel.Content = string.Empty;
            entry = new MathBoxLogEntry();
         }
         else
         {
            recordNumberLabel.Content = this.recordNumber;
         }

         pc.Field = entry.GetAttribute("PC");

         addressIn.Field = entry.GetAttribute("AddressIn");
         dataIn.Field = entry.GetAttribute("DataIn");
         begin.Field = entry.GetAttribute("BEGIN");
         j.Field = entry.GetAttribute("J");
         pcen.Field = entry.GetAttribute("PCEN");
         s.Field = entry.GetAttribute("S");
         s0.Field = entry.GetAttribute("S0");
         s1.Field = entry.GetAttribute("S1");
         jumpLatch.Field = entry.GetAttribute("JumpLatch");

         aluE.LogEntry = entry.GetALU("E");
         aluF.LogEntry = entry.GetALU("F");
         aluJ.LogEntry = entry.GetALU("J");
         aluK.LogEntry = entry.GetALU("K");
      }
   }
}
