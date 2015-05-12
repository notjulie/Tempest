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
         // normal initialization
         InitializeComponent();

         // event handlers
         breakPoint.Checked += breakpointCheckChanged;
         breakPoint.Unchecked += breakpointCheckChanged;
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

      /// <summary>
      /// Gets the address associated with the line, -1 if this is not an actual code line
      /// </summary>
      public int Address
      {
         get
         {
            string s = Text;
            if (s.Length < 4)
               return -1;
            return ParseHex(s.Substring(0, 4));
         }
      }

      /// <summary>
      /// Get or sets a value indicating whether this line is a breakpoint
      /// </summary>
      public bool IsBreakpoint
      {
         get
         {
            return breakPoint.IsChecked == true;
         }
         set
         {
            breakPoint.IsChecked = value;
         }
      }

      /// <summary>
      /// Event fired when the line's breakpoint property changes
      /// </summary>
      public event RoutedEventHandler BreakpointChanged;

      private void breakpointCheckChanged(object sender, RoutedEventArgs e)
      {
         if (BreakpointChanged != null)
            BreakpointChanged(this, e);
      }

      private int ParseHex(string s)
      {
         int result = 0;
         for (int i=0; i<s.Length; ++i)
         {
            int digit;
            if (s[i] >= '0' && s[i] <= '9')
               digit = s[i] - '0';
            else if (s[i] >= 'a' && s[i] <= 'f')
               digit = 10 + s[i] - 'a';
            else if (s[i] >= 'A' && s[i] <= 'F')
               digit = 10 + s[i] - 'A';
            else
               return -1;
            result = 16 * result + digit;
         }

         return result;
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
