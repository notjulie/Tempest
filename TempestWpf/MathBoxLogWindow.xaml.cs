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
using System.Windows.Shapes;

namespace TempestWpf
{
   /// <summary>
   /// Interaction logic for MathBoxLog.xaml
   /// </summary>
   public partial class MathBoxLogWindow : Window
   {
      private MathBoxLog mathBoxLog = new MathBoxLog();
      private int leftIndex;

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class MathBoxLog
      /// </summary>
      public MathBoxLogWindow()
      {
         // normal initialization
         InitializeComponent();

         // event handlers
         Closing += MathBoxLogWindow_Closing;
         pageLeftButton.Click += pageLeftButton_Click;
         leftButton.Click += leftButton_Click;
         rightButton.Click += rightButton_Click;
         pageRightButton.Click += pageRightButton_Click;
      }

      void MathBoxLogWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
      {
         e.Cancel = true;
         this.Visibility = System.Windows.Visibility.Hidden;
      }

      #endregion

      /// <summary>
      /// Gets or sets the log data
      /// </summary>
      public MathBoxLog MathBoxLog
      {
         get
         {
            return mathBoxLog;
         }
         set
         {
            // accept the value
            mathBoxLog = value;

            // set our scroll index to display the last two entries if we have that many
            leftIndex = mathBoxLog.Entries.Length - 2;

            // display the entries
            UpdateEntries();
         }
      }

      #region Event Handlers

      void leftButton_Click(object sender, RoutedEventArgs e)
      {
         if (leftIndex > 0)
         {
            --leftIndex;
            UpdateEntries();
         }
      }

      void rightButton_Click(object sender, RoutedEventArgs e)
      {
         if (leftIndex < mathBoxLog.Entries.Length - 1)
         {
            ++leftIndex;
            UpdateEntries();
         }
      }

      void pageRightButton_Click(object sender, RoutedEventArgs e)
      {
         if (leftIndex < mathBoxLog.Entries.Length - 11)
         {
            leftIndex += 10;
            UpdateEntries();
         }
      }

      void pageLeftButton_Click(object sender, RoutedEventArgs e)
      {
         if (leftIndex > 10)
         {
            leftIndex -= 10;
            UpdateEntries();
         }
      }

      #endregion

      private void UpdateEntries()
      {
         SetPanelEntry(logEntry1, leftIndex);
         SetPanelEntry(logEntry2, leftIndex + 1);
      }

      private void SetPanelEntry(MathBoxLogEntryControl logControl, int index)
      {
         if (index < 0 || index >= mathBoxLog.Entries.Length)
         {
            logControl.Entry = null;
         }
         else
         {
            logControl.Entry = mathBoxLog.Entries[index];
            logControl.RecordNumber = index + 1;
         }
      }
   }
}
