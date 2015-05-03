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

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class MathBoxLog
      /// </summary>
      public MathBoxLogWindow()
      {
         InitializeComponent();
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
            mathBoxLog = value;
         }
      }
   }
}
