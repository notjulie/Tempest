using System;
using System.Collections.Generic;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

using TempestDotNET;

namespace TempestWpf
{
   /// <summary>
   /// Interaction logic for MainWindow.xaml
   /// </summary>
   public partial class MainWindow : Window
   {
      #region Private Fields

      private Tempest tempest;
      private DispatcherTimer timer;
      private DispatcherTimer vectorTimer;
      private DispatcherTimer spinnerTimer;
      private List<Line> lines = new List<Line>();
      private DateTime startTime;
      private Debug6502Window debug6502;
      private SolidColorBrush[] vectorBrush = new SolidColorBrush[16];
      private bool leftKeyDown = false;
      private bool rightKeyDown = false;

      #endregion

      /// <summary>
      /// Initializes a new instance of class MainWindow
      /// </summary>
      public MainWindow()
      {
         // normal initialization
         InitializeComponent();

         // set up our vector brushes
         vectorBrush[0] = new SolidColorBrush(Colors.White);
         vectorBrush[1] = new SolidColorBrush(Colors.Yellow);
         vectorBrush[2] = new SolidColorBrush(Colors.Purple);
         vectorBrush[3] = new SolidColorBrush(Colors.Red);
         vectorBrush[4] = new SolidColorBrush(Colors.Orange);
         vectorBrush[5] = new SolidColorBrush(Colors.Lime);
         vectorBrush[6] = new SolidColorBrush(Colors.Blue);
         vectorBrush[7] = new SolidColorBrush(Colors.Pink);
         vectorBrush[8] = new SolidColorBrush(Colors.Gray);
         vectorBrush[9] = new SolidColorBrush(Colors.Maroon);
         vectorBrush[10] = new SolidColorBrush(Colors.Gold);
         vectorBrush[11] = new SolidColorBrush(Colors.Silver);
         vectorBrush[12] = new SolidColorBrush(Colors.Fuchsia);
         vectorBrush[13] = new SolidColorBrush(Colors.LightBlue);
         vectorBrush[14] = new SolidColorBrush(Colors.LightGreen);
         vectorBrush[15] = new SolidColorBrush(Colors.LightGray);

         // event handlers
         this.Loaded += MainWindow_Loaded;
         this.Closed += MainWindow_Closed;
         this.Closing += MainWindow_Closing;
         this.KeyDown += MainWindow_KeyDown;
         this.KeyUp += MainWindow_KeyUp;
         buttonOnePlayerStart.Click += buttonOnePlayerStart_Click;
         view6502DebugWindowItem.Click += view6502DebugWindowItem_Click;
      }


      void MainWindow_KeyDown(object sender, KeyEventArgs e)
      {
         switch (e.Key)
         {
            case Key.Left:
               tempest.MoveWheel(-4);
               leftKeyDown = true;
               rightKeyDown = false;
               break;

            case Key.Right:
               tempest.MoveWheel(4);
               leftKeyDown = false;
               rightKeyDown = true;
               break;

            case Key.F:
               tempest.Fire(true);
               break;

            case Key.V:
               tempest.Zap(true);
               break;
         }
      }

      void MainWindow_KeyUp(object sender, KeyEventArgs e)
      {
         switch (e.Key)
         {
            case Key.Left:
               leftKeyDown = false;
               break;

            case Key.Right:
               rightKeyDown = false;
               break;

            case Key.F:
               tempest.Fire(false);
               break;

            case Key.V:
               tempest.Zap(false);
               break;
         }
      }

      void view6502DebugWindowItem_Click(object sender, RoutedEventArgs e)
      {
         // create the window if we haven't yet
         if (debug6502 == null)
         {
            debug6502 = new Debug6502Window();
            debug6502.Tempest = tempest;
         }

         // show it
         debug6502.Show();
         debug6502.Activate();
      }

      void buttonOnePlayerStart_Click(object sender, RoutedEventArgs e)
      {
         tempest.SetOnePlayerButton(true);
         System.Threading.Thread.Sleep(100);
         tempest.SetOnePlayerButton(false);
      }

      void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
      {
         timer.IsEnabled = false;
         vectorTimer.IsEnabled = false;
         e.Cancel = false;
      }

      void MainWindow_Closed(object sender, EventArgs e)
      {
         tempest.Dispose();
      }

      void MainWindow_Loaded(object sender, RoutedEventArgs e)
      {
         // create our tempest
         tempest = new Tempest();

         // set it to running
         startTime = DateTime.Now;
         tempest.Start();

         timer = new DispatcherTimer();
         timer.Interval = TimeSpan.FromMilliseconds(200);
         timer.IsEnabled = true;
         timer.Tick += timer_Tick;

         vectorTimer = new DispatcherTimer();
         vectorTimer.Interval = TimeSpan.FromMilliseconds(50);
         vectorTimer.IsEnabled = true;
         vectorTimer.Tick += vectorTimer_Tick;

         spinnerTimer = new DispatcherTimer();
         spinnerTimer.Interval = TimeSpan.FromMilliseconds(50);
         spinnerTimer.IsEnabled = true;
         spinnerTimer.Tick += spinnerTimer_Tick;
      }

      void spinnerTimer_Tick(object sender, EventArgs e)
      {
         if (leftKeyDown)
            tempest.MoveWheel(-1);
         else if (rightKeyDown)
            tempest.MoveWheel(1);
      }

      void timer_Tick(object sender, EventArgs e)
      {
         processorStatus.Text = tempest.GetProcessorStatus();

         double processorSpeed = tempest.GetTotalClockCycles();
         processorSpeed /= (DateTime.Now - startTime).TotalSeconds;
         processorSpeed /= 1000000;
         processorSpeedText.Text = processorSpeed.ToString("F1") + " MHz";

         double irqFrequencyValue = tempest.GetIRQCount();
         irqFrequencyValue /= (DateTime.Now - startTime).TotalSeconds;
         irqFrequency.Text = irqFrequencyValue.ToString("F1");
      }

      void vectorTimer_Tick(object sender, EventArgs e)
      {
         // get a vector enumerator
         VectorEnumerator enumerator = tempest.GetVectorEnumerator();
         if (enumerator != null)
         {
            int index = 0;
            for (; ; )
            {
               // get the vector
               Int16 startX16, startY16, endX16, endY16;
               int color;
               if (!enumerator.GetNextVector(out startX16, out startY16, out endX16, out endY16, out color))
                  break;

               // switch to longer than 16 bits
               int startX = startX16;
               int startY = startY16;
               int endX = endX16;
               int endY = endY16;

               Line line;
               if (index >= lines.Count)
               {
                  line = new Line();
                  lines.Add(line);
                  canvas.Children.Add(line);
               }
               else
               {
                  line = lines[index];
               }

               int strokeThickness = 100;
               
               // if we have a zero length line we need to fudge it... this is how
               // Tempest draws points
               if (endX == startX && endY==startY)
               {
                  endX += strokeThickness / 2;
                  startX += strokeThickness / 2;
                  endY -= strokeThickness / 2;
                  startY += strokeThickness / 2;
               }

               line.Stroke = vectorBrush[color];
               line.X1 = canvas.ActualWidth / 2 + startX;
               line.X2 = canvas.ActualWidth / 2 + endX;
               line.Y1 = canvas.ActualHeight / 2 - startY;
               line.Y2 = canvas.ActualHeight / 2 - endY;
               line.StrokeThickness = 100;
               line.Visibility = System.Windows.Visibility.Visible;

               ++index;
            }

            while (index < lines.Count)
               lines[index++].Visibility = System.Windows.Visibility.Hidden;
         }
      }

      private byte[] GetROM(string name)
      {
         Assembly assembly = Assembly.GetExecutingAssembly();
         string[] names = assembly.GetManifestResourceNames();
         for (int i = 0; i < names.Length; ++i)
         {
            if (names[i].IndexOf(name) >= 0)
            {
               Stream stream = assembly.GetManifestResourceStream(names[i]);
               List<byte> rom = new List<byte>();
               for (; ; )
               {
                  int b = stream.ReadByte();
                  if (b < 0)
                     break;
                  rom.Add((byte)b);
               }
               return rom.ToArray();
            }
         }

         throw new Exception("ROM resource not found: " + name);
      }
   }
}
