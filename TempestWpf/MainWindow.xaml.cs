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
      private DispatcherTimer audioTimer;
      private DispatcherTimer vectorTimer;
      private List<Line> lines = new List<Line>();
      private DateTime startTime;
      private Debug6502Window debug6502;
      private SolidColorBrush[] vectorBrush = new SolidColorBrush[16];

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
               break;

            case Key.Right:
               tempest.MoveWheel(4);
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
               tempest.MoveWheel(-4);
               break;

            case Key.Right:
               tempest.MoveWheel(4);
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

         // load its ROMs
         tempest.LoadROM(GetROM("136002.113"), 0x9000);
         tempest.LoadROM(GetROM("136002.114"), 0x9800);
         tempest.LoadROM(GetROM("136002.115"), 0xA000);
         tempest.LoadROM(GetROM("136002.116"), 0xA800);
         tempest.LoadROM(GetROM("136002.117"), 0xB000);
         tempest.LoadROM(GetROM("136002.118"), 0xB800);
         tempest.LoadROM(GetROM("136002.119"), 0xC000);
         tempest.LoadROM(GetROM("136002.120"), 0xC800);
         tempest.LoadROM(GetROM("136002.121"), 0xD000);
         tempest.LoadROM(GetROM("136002.122"), 0xD800);

         // load the Vector ROM, too
         tempest.LoadROM(GetROM("136002.111"), 0x3000);
         tempest.LoadROM(GetROM("136002.112"), 0x3800);

         // and the math box ROM
         tempest.LoadMathBoxROM(GetROM("136002.126"), (sbyte)'A');
         tempest.LoadMathBoxROM(GetROM("136002.127"), (sbyte)'E');
         tempest.LoadMathBoxROM(GetROM("136002.128"), (sbyte)'F');
         tempest.LoadMathBoxROM(GetROM("136002.129"), (sbyte)'H');
         tempest.LoadMathBoxROM(GetROM("136002.130"), (sbyte)'J');
         tempest.LoadMathBoxROM(GetROM("136002.131"), (sbyte)'K');
         tempest.LoadMathBoxROM(GetROM("136002.132"), (sbyte)'L');

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

         audioTimer = new DispatcherTimer();
         audioTimer.Interval = TimeSpan.FromMilliseconds(100);
         audioTimer.IsEnabled = true;
         audioTimer.Tick += audioTimer_Tick;
      }

      void audioTimer_Tick(object sender, EventArgs e)
      {
         UInt16[] audioStatus = tempest.GetAudioChannelsStatus();
         s1Volume.Text = audioStatus[0].ToString();
         s1Waveform.Text = audioStatus[1].ToString();
         s1Frequency.Text = audioStatus[2].ToString();
         s2Volume.Text = audioStatus[3].ToString();
         s2Waveform.Text = audioStatus[4].ToString();
         s2Frequency.Text = audioStatus[5].ToString();
         s3Volume.Text = audioStatus[6].ToString();
         s3Waveform.Text = audioStatus[7].ToString();
         s3Frequency.Text = audioStatus[8].ToString();
         s4Volume.Text = audioStatus[9].ToString();
         s4Waveform.Text = audioStatus[10].ToString();
         s4Frequency.Text = audioStatus[11].ToString();
         s5Volume.Text = audioStatus[12].ToString();
         s5Waveform.Text = audioStatus[13].ToString();
         s5Frequency.Text = audioStatus[14].ToString();
         s6Volume.Text = audioStatus[15].ToString();
         s6Waveform.Text = audioStatus[16].ToString();
         s6Frequency.Text = audioStatus[17].ToString();
         s7Volume.Text = audioStatus[18].ToString();
         s7Waveform.Text = audioStatus[19].ToString();
         s7Frequency.Text = audioStatus[20].ToString();
         s8Volume.Text = audioStatus[21].ToString();
         s8Waveform.Text = audioStatus[22].ToString();
         s8Frequency.Text = audioStatus[23].ToString();
      }

      void timer_Tick(object sender, EventArgs e)
      {
         processorStatus.Text = tempest.GetProcessorStatus();
         audioStatus.Text = tempest.GetAudioStatus();

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
               float startX, startY, endX, endY;
               int color;
               if (!enumerator.GetNextVector(out startX, out startY, out endX, out endY, out color))
                  break;

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

               // if we have a zero length line we need to fudge it... this is how
               // Tempest draws points
               if (endX == startX && endY==startY)
               {
                  endX -= 0.5F;
                  startX += 0.5F;
                  endY -= 0.5F;
                  startY += 0.5F;
               }

               line.Stroke = vectorBrush[color];
               line.X1 = canvas.ActualWidth / 2 + startX;
               line.X2 = canvas.ActualWidth / 2 + endX;
               line.Y1 = canvas.ActualHeight / 2 - startY;
               line.Y2 = canvas.ActualHeight / 2 - endY;
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
