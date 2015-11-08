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

      // our Tempest objects
      private TDNWin32TempestSoundIO tempestSoundIO;
      private TDNWin32TempestVectorIO tempestVectorIO;
      private Tempest tempest;
      private TDNComPortStream tempestComPortStream;
      private TDNIOStreamProxy tempestIOStreamProxy;

      private DispatcherTimer timer;
      private DispatcherTimer vectorTimer;
      private DispatcherTimer spinnerTimer;
      private List<Line> lines = new List<Line>();
      private DateTime startTime;
      private SolidColorBrush[] vectorBrush = new SolidColorBrush[16];
      private bool leftKeyDown = false;
      private bool rightKeyDown = false;

      private SolidColorBrush ledOnBrush = new SolidColorBrush(Colors.Red);
      private SolidColorBrush ledOffBrush = new SolidColorBrush(Colors.Black);

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
         onePlayer.MouseLeftButtonDown += onePlayer_MouseLeftButtonDown;
         onePlayer.MouseLeftButtonUp += onePlayer_MouseLeftButtonUp;
      }

      void onePlayer_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
      {
         // this goes to our simulated sound IO device
         tempestSoundIO.OnePlayer(true);
      }

      void onePlayer_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
      {
         tempestSoundIO.OnePlayer(false);
      }

      void MainWindow_KeyDown(object sender, KeyEventArgs e)
      {
         switch (e.Key)
         {
            case Key.Left:
               tempestSoundIO.MoveWheel(-4);
               leftKeyDown = true;
               rightKeyDown = false;
               break;

            case Key.Right:
               tempestSoundIO.MoveWheel(4);
               leftKeyDown = false;
               rightKeyDown = true;
               break;

            case Key.F:
               tempestSoundIO.Fire(true);
               break;

            case Key.V:
               tempestSoundIO.Zap(true);
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
               tempestSoundIO.Fire(false);
               break;

            case Key.V:
               tempestSoundIO.Zap(false);
               break;
         }
      }

      void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
      {
         if (timer != null)
            timer.IsEnabled = false;
         if (vectorTimer != null)
            vectorTimer.IsEnabled = false;
         e.Cancel = false;
      }

      void MainWindow_Closed(object sender, EventArgs e)
      {
         if (tempest != null)
         {
            tempest.Dispose();
            tempest = null;
         }
      }

      void MainWindow_Loaded(object sender, RoutedEventArgs e)
      {
         try
         {
            // create the stream object that will connect to the sound board
            tempestComPortStream = new TDNComPortStream("COM3");

            // create the IO object that we represent
            tempestVectorIO = new TDNWin32TempestVectorIO();
            tempestSoundIO = new TDNWin32TempestSoundIO();

            // create the IO proxy
            tempestIOStreamProxy = new TDNIOStreamProxy(tempestComPortStream);

            // create our tempest
            tempest = new Tempest(tempestIOStreamProxy, tempestVectorIO);

            // at this point here's what we have:
            //   - tempest is writing to tempestIOStreamProxy, which is its output device
            //   - tempestIOStreamProxy is writing to tempestMemoryStream
            //   - tempestIOStreamListener is reading from tempestMemoryStream and writing
            //     to tempestIO, which is the actual screen output device
            //
            // this is basically the identical scheme that we'll use to write to the real device
            //

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
            spinnerTimer.Interval = TimeSpan.FromMilliseconds(10);
            spinnerTimer.IsEnabled = true;
            spinnerTimer.Tick += spinnerTimer_Tick;
         }
         catch (Exception x)
         {
            MessageBox.Show(x.Message);
            Close();
         }
      }

      void spinnerTimer_Tick(object sender, EventArgs e)
      {
         if (leftKeyDown)
            tempestSoundIO.MoveWheel(-1);
         else if (rightKeyDown)
            tempestSoundIO.MoveWheel(1);

         // update our LED's
         if (tempestSoundIO.OnePlayerLED())
            onePlayer.Fill = ledOnBrush;
         else
            onePlayer.Fill = ledOffBrush;
         if (tempestSoundIO.TwoPlayerLED())
            twoPlayer.Fill = ledOnBrush;
         else
            twoPlayer.Fill = ledOffBrush;
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
         VectorEnumerator enumerator = tempestVectorIO.GetVectorEnumerator();
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
