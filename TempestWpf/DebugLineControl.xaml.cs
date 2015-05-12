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
   /// Interaction logic for DebugLineControl.xaml
   /// </summary>
   public partial class DebugLineControl : UserControl
   {
      /// <summary>
      /// The Text dependency property
      /// </summary>
      public static readonly DependencyProperty TextProperty = DependencyProperty.Register(
        "Text",
        typeof(String),
        typeof(DebugLineControl),
        new PropertyMetadata(null, new PropertyChangedCallback(OnTextChanged))
      );

      /// <summary>
      /// Initializes a new instance of class DebugLineControl
      /// </summary>
      public DebugLineControl()
      {
         InitializeComponent();
      }

      /// <summary>
      /// Gets or sets the text
      /// </summary>
      public string Text
      {
         get
         {
            return (String)GetValue(TextProperty);
         }
         set
         {
            SetValue(TextProperty, value);
            text.Text = value;
         }
      }


      private static void OnTextChanged(
        DependencyObject d, 
        DependencyPropertyChangedEventArgs e
      )
      {
         ((DebugLineControl)d).text.Text = e.NewValue.ToString();
      }
   }
}
