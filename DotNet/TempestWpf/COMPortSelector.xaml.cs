using System;
using System.IO.Ports;
using System.Windows.Controls;
using System.Windows.Threading;

namespace TempestWpf
{
   /// <summary>
   /// Interaction logic for COMPortSelector.xaml
   /// </summary>
   public partial class COMPortSelector : UserControl
   {
      private DispatcherTimer timer;
      private string portName = string.Empty;
      private bool preventSelectionUpdate = false;

      /// <summary>
      /// Initializes a new instance of class COMPortSelector
      /// </summary>
      public COMPortSelector()
      {
         // normal component initialization
         InitializeComponent();

         // add our timer
         timer = new DispatcherTimer();
         timer.Interval = TimeSpan.FromSeconds(5);
         timer.Tick += Timer_Tick;
         timer.IsEnabled = true;

         // add event handlers
         combo.SelectionChanged += Combo_SelectionChanged;

         // initially populate
         Populate();
      }

      private void Combo_SelectionChanged(object sender, SelectionChangedEventArgs e)
      {
         if (!preventSelectionUpdate)
            if (combo.SelectedIndex >= 0)
               portName = combo.Items[combo.SelectedIndex].ToString();
      }

      /// <summary>
      /// Gets or sets the header text
      /// </summary>
      public string Header
      {
         get
         {
            return header.Content.ToString();
         }
         set
         {
            header.Content = value;
         }
      }

      /// <summary>
      /// Gets or sets the selected port name
      /// </summary>
      public string PortName
      {
         get
         {
            return portName;
         }
         set
         {
            portName = value;
            Populate();
         }
      }

      private void Timer_Tick(object sender, EventArgs e)
      {
         Populate();
      }

      private void Populate()
      {
         try
         {
            preventSelectionUpdate = true;

            // sort items
            combo.Items.IsLiveSorting = true;

            // get serial port names
            var names = SerialPort.GetPortNames();

            // add names that aren't present
            foreach (string name in names)
            {
               bool found = false;
               for (int i = 0; i < combo.Items.Count; ++i)
               {
                  if (combo.Items[i].ToString() == name)
                     found = true;
               }

               if (!found)
                  combo.Items.Add(name);
            }

            // remove names that shouldn't be present
            for (int i = combo.Items.Count - 1; i >= 0; --i)
            {
               bool found = false;
               foreach (string name in names)
                  if (combo.Items[i].ToString() == name)
                     found = true;
               if (!found)
                  combo.Items.RemoveAt(i);
            }

            // make sure the right one is selected
            for (int i = 0; i < combo.Items.Count; ++i)
               if (combo.Items[i].ToString() == portName)
                  combo.SelectedIndex = i;
         }
         finally
         {
            preventSelectionUpdate = false;
         }
      }
   }
}
