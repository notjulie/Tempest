// ====================================================================
// Tempest WPF version
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Handy little user control for named values.
// ====================================================================

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
   /// Interaction logic for LabeledField.xaml
   /// </summary>
   public partial class LabeledField : UserControl
   {
      /// <summary>
      /// Initializes a new instance of class LabeledField
      /// </summary>
      public LabeledField()
      {
         InitializeComponent();
      }

      /// <summary>
      /// Gets or sets the field text
      /// </summary>
      public string Field
      {
         get
         {
            return field.Text;
         }
         set
         {
            field.Text = value;
         }
      }

      /// <summary>
      /// Gets or sets the label text
      /// </summary>
      public string LabelText
      {
         get
         {
            return label.Text;
         }
         set
         {
            label.Text = value;
         }
      }
   }
}
