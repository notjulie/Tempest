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
      private Tempest tempest;
      private DispatcherTimer timer;
      private DispatcherTimer vectorTimer;
      private List<Line> lines = new List<Line>();
      private DateTime startTime;

      public MainWindow()
      {
         // normal initialization
         InitializeComponent();

         // event handlers
         this.Loaded += MainWindow_Loaded;
         this.Closed += MainWindow_Closed;
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
         vectorTimer.Interval = TimeSpan.FromMilliseconds(200);
         vectorTimer.IsEnabled = true;
         vectorTimer.Tick += vectorTimer_Tick;
      }

      void timer_Tick(object sender, EventArgs e)
      {
         processorStatus.Text = tempest.GetProcessorStatus();
         mathBoxStatus.Text = tempest.GetMathBoxStatus();

         double processorSpeed = tempest.GetTotalClockCycles();
         processorSpeed /= (DateTime.Now - startTime).TotalSeconds;
         processorSpeed /= 1000000;
         processorSpeedText.Text = processorSpeed.ToString("F1") + " MHz";
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

               line.Stroke = new SolidColorBrush(Colors.White);
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
