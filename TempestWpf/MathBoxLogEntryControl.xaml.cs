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

      private void UpdateControls()
      {
         pc.Content = entry.GetAttribute("PC");
      }
   }
}
