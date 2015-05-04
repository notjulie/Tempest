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

         a.Field = entry.GetAttribute("A");
         b.Field = entry.GetAttribute("B");
         dataIn.Field = entry.GetAttribute("DataIn");
         cn.Field = entry.GetAttribute("Cn");
         f3.Field = entry.GetAttribute("F3");
         i012.Field = entry.GetAttribute("I012");
         i678.Field = entry.GetAttribute("I678");
         ovr.Field = entry.GetAttribute("OVR");
         r.Field = entry.GetAttribute("R");
         s.Field = entry.GetAttribute("S");
         qLatch.Field = entry.GetAttribute("QLatch");

         // set i345
         string i345Value = entry.GetAttribute("I345");
         switch (i345Value)
         {
            case "00": i345.Field = "ADD (0)"; break;
            case "01": i345.Field = "SUBR (1)"; break;
            case "02": i345.Field = "SUBS (2)"; break;
            case "03": i345.Field = "OR (3)"; break;
            case "04": i345.Field = "AND (4)"; break;
            case "05": i345.Field = "NOTRS (5)"; break;
            case "06": i345.Field = "EXOR (6)"; break;
            case "07": i345.Field = "EXNOR (7)"; break;
            default: i345.Field = i345Value; break;
         }

         // RAM
         ram0.Text = entry.GetAttribute("RAM0");
         ram1.Text = entry.GetAttribute("RAM1");
         ram2.Text = entry.GetAttribute("RAM2");
         ram3.Text = entry.GetAttribute("RAM3");
         ram4.Text = entry.GetAttribute("RAM4");
         ram5.Text = entry.GetAttribute("RAM5");
         ram6.Text = entry.GetAttribute("RAM6");
         ram7.Text = entry.GetAttribute("RAM7");
         ram8.Text = entry.GetAttribute("RAM8");
         ram9.Text = entry.GetAttribute("RAM9");
         ramA.Text = entry.GetAttribute("RAMA");
         ramB.Text = entry.GetAttribute("RAMB");
         ramC.Text = entry.GetAttribute("RAMC");
         ramD.Text = entry.GetAttribute("RAMD");
         ramE.Text = entry.GetAttribute("RAME");
         ramF.Text = entry.GetAttribute("RAMF");
      }
   }
}
