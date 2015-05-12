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
      private static SolidColorBrush selectedBrush = new SolidColorBrush(Colors.Pink);
      private static SolidColorBrush unselectedBrush = new SolidColorBrush(Colors.White);

      /// <summary>
      /// The Item dependency property
      /// </summary>
      public static readonly DependencyProperty ItemProperty = DependencyProperty.Register(
        "Item",
        typeof(DebugLine),
        typeof(DebugLineControl),
        new PropertyMetadata(new DebugLine(String.Empty), new PropertyChangedCallback(OnItemChanged))
      );

      /// <summary>
      /// Initializes a new instance of class DebugLineControl
      /// </summary>
      public DebugLineControl()
      {
         // normal initialization
         InitializeComponent();

         // set initial state
         Update();

         // event handlers
         breakPoint.Checked += breakpointCheckChanged;
         breakPoint.Unchecked += breakpointCheckChanged;
      }

      /// <summary>
      /// Gets or sets the data item
      /// </summary>
      public DebugLine Item
      {
         get
         {
            return (DebugLine)GetValue(ItemProperty);
         }
         set
         {
            SetValue(ItemProperty, value);
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

      private void Update()
      {
         DebugLine item = Item;
         if (item == null)
            item = new DebugLine(String.Empty);
         text.Text = item.Text;
         Background = item.IsSelected ? selectedBrush : unselectedBrush;
         breakPoint.Visibility = item.Address >= 0 ? Visibility.Visible : Visibility.Hidden;
      }

      private static void OnItemChanged(
        DependencyObject d,
        DependencyPropertyChangedEventArgs e
      )
      {
         ((DebugLineControl)d).Update();
      }
   }
}
