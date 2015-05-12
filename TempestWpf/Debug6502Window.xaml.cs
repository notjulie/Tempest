using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Threading;

using TempestDotNET;

namespace TempestWpf
{
   /// <summary>
   /// Interaction logic for Debug6502Window.xaml
   /// </summary>
   public partial class Debug6502Window : Window
   {
      #region Private Fields

      private Tempest tempest;
      private DispatcherTimer timer;
      private bool stopped = false;
      private DebugLine selectedLine;
      private ObservableCollection<DebugLine> items;

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class Debug6502Window
      /// </summary>
      public Debug6502Window()
      {
         // normal initialization
         InitializeComponent();

         // create the timer... we run even when not showing
         timer = new DispatcherTimer();
         timer.Interval = TimeSpan.FromMilliseconds(50);
         timer.IsEnabled = true;
         timer.Tick += timer_Tick;

         // event handlers
         Closing += Debug6502Window_Closing;

         // load the disassembly
         List<string> disassembly = new List<string>();
         Assembly assembly = Assembly.GetExecutingAssembly();
         string[] names = assembly.GetManifestResourceNames();
         for (int i = 0; i < names.Length; ++i)
         {
            if (names[i].IndexOf("Disassembled") >= 0)
            {
               using (StreamReader reader = new StreamReader(assembly.GetManifestResourceStream(names[i])))
               {
                  for (;;)
                  {
                     string line = reader.ReadLine();
                     if (line == null)
                        break;
                     disassembly.Add(line);
                  }
               }
            }
         }

         // fill the list box with it
         items = new ObservableCollection<DebugLine>();
         for (int i=0; i<disassembly.Count; ++i)
         {
            DebugLine item = new DebugLine(disassembly[i]);
            items.Add(item);
         }

         listView.ItemsSource = items;
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets or sets the Tempest instance
      /// </summary>
      public Tempest Tempest
      {
         get
         {
            return tempest;
         }
         set
         {
            // never mind if this is not a change
            if (value == tempest)
               return;

            // set the value
            tempest = value;

            // mark ourself as not stopped, so that if it is the timer will
            // act like we just hit a break point and bring us to the front
            stopped = false;
         }
      }

      #endregion

      #region Event Handlers

      void timer_Tick(object sender, EventArgs e)
      {
         if (Tempest == null)
            return;

         // if we changed from running to not running show ourself and
         // highlight the current instruction
         if (!stopped && tempest.IsStopped())
         {
            stopped = true;

            this.Show();
            this.Activate();

            ShowCurrentInstruction();
         }
      }

      void Debug6502Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
      {
         e.Cancel = true;
         this.Visibility = System.Windows.Visibility.Hidden;
      }

      private void itemBreakpointChanged(object sender, RoutedEventArgs e)
      {
         DebugLineControl lineControl = sender as DebugLineControl;
         if (lineControl == null)
            return;

         int address = lineControl.Item.Address;
         if (address < 0)
            return;

         Tempest.SetBreakpoint(address, lineControl.IsBreakpoint);
      }

      #endregion

      #region Private Methods

      private void SelectLine(DebugLine line)
      {
         if (selectedLine != null)
         {
            selectedLine.IsSelected = false;
            UpdateItem(selectedLine);
         }
         selectedLine = line;
         if (selectedLine != null)
         {
            selectedLine.IsSelected = true;
            UpdateItem(selectedLine);
         }
      }
      
      private void UpdateItem(DebugLine line)
      {
         int i = items.IndexOf(line);
         if (i >= 0)
         {
            items[i] = null;
            items[i] = line;
         }
      }

      private void ShowCurrentInstruction()
      {
         int pc = tempest.GetProgramCounter();
         for (int i=0; i<listView.Items.Count; ++i)
         {
            DebugLine line = (DebugLine)listView.Items[i];
            if (line.Address == pc)
            {
               SelectLine(line);
               listView.ScrollIntoView(line);
               break;
            }
         }
      }

      #endregion
   }
}
