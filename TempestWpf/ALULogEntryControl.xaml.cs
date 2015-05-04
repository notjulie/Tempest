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
   /// Interaction logic for ALULogEntryControl.xaml
   /// </summary>
   public partial class ALULogEntryControl : UserControl
   {
      private ALULogEntry logEntry;

      /// <summary>
      /// Initializes a new instance of class ALULogEntryControl
      /// </summary>
      public ALULogEntryControl()
      {
         InitializeComponent();
      }

      /// <summary>
      /// Gets or sets the log entry
      /// </summary>
      public ALULogEntry LogEntry
      {
         get
         {
            return logEntry;
         }
         set
         {
            logEntry = value;
            UpdateControls();
         }
      }

      private void UpdateControls()
      {
         // special case if our entry is null
         ALULogEntry entry = this.logEntry;
         if (entry == null)
            entry = new ALULogEntry();

         f3.Content = entry.GetAttribute("F3");
         ovr.Content = entry.GetAttribute("OVR");
      }
   }
}
