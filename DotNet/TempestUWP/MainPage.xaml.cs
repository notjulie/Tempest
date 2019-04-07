using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Shapes;
using Windows.UI.Xaml.Navigation;

using Tempest_UWP_Component;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace TempestUWP
{
   /// <summary>
   /// An empty page that can be used on its own or navigated to within a Frame.
   /// </summary>
   public sealed partial class MainPage : Page
   {
      private VectorGameManager vectorGameManager;
      private SolidColorBrush[] vectorBrush = new SolidColorBrush[16];
      private DateTime startTime;
      private DispatcherTimer vectorTimer;
      private List<Line> lines = new List<Line>();

      public MainPage()
      {
         this.InitializeComponent();

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
         this.Unloaded += MainPage_Unloaded;
         this.KeyDown += MainPage_KeyDown;
         this.KeyUp += MainWindow_KeyUp;
         //buttonOnePlayerStart.MouseLeftButtonDown += buttonOnePlayerStart_MouseLeftButtonDown;
         //buttonTwoPlayerStart.MouseLeftButtonDown += buttonTwoPlayerStart_MouseLeftButtonDown;
         //buttonMenu.Click += ButtonMenu_Click;
      }

      private void MainPage_Unloaded(object sender, RoutedEventArgs e)
      {
         if (vectorGameManager != null)
         {
            vectorGameManager.Dispose();
            vectorGameManager = null;
         }
      }

      void MainPage_KeyDown(object sender, KeyRoutedEventArgs e)
      {
         /*switch (e.Key)
         {
            case Key.Left:
               vectorGameManager.MoveWheel(-4);
               leftKeyDown = true;
               rightKeyDown = false;
               break;

            case Key.Right:
               vectorGameManager.MoveWheel(4);
               leftKeyDown = false;
               rightKeyDown = true;
               break;

            case Key.F:
               vectorGameManager.Fire(true);
               break;

            case Key.V:
               vectorGameManager.Zap(true);
               break;
         }*/
      }

      void MainWindow_KeyUp(object sender, KeyRoutedEventArgs e)
      {
         /*switch (e.Key)
         {
            case Key.Left:
               leftKeyDown = false;
               break;

            case Key.Right:
               rightKeyDown = false;
               break;

            case Key.F:
               vectorGameManager.Fire(false);
               break;

            case Key.V:
               vectorGameManager.Zap(false);
               break;
         }*/
      }

      private void MainWindow_Loaded(object sender, RoutedEventArgs e)
      {
         RestartGame();

         /*timer = new DispatcherTimer();
         timer.Interval = TimeSpan.FromMilliseconds(200);
         timer.IsEnabled = true;
         timer.Tick += timer_Tick;*/

         vectorTimer = new DispatcherTimer();
         vectorTimer.Interval = TimeSpan.FromMilliseconds(15);
         vectorTimer.Tick += VectorTimer_Tick;
         vectorTimer.Start();

         /*spinnerTimer = new DispatcherTimer();
         spinnerTimer.Interval = TimeSpan.FromMilliseconds(10);
         spinnerTimer.IsEnabled = true;
         spinnerTimer.Tick += spinnerTimer_Tick;*/
      }

      private void RestartGame()
      {
         // dispose the old game
         if (vectorGameManager != null)
         {
            vectorGameManager.Dispose();
            vectorGameManager = null;
         }

         // create our tempest... just a normal game that interacts with our keyboard
         // commands and the internal audio
         vectorGameManager = VectorGameManager.CreateNormalInstance();

         // set it to running
         startTime = DateTime.Now;
         vectorGameManager.Start();
      }

      void VectorTimer_Tick(object sender, object e)
      {
         if (vectorGameManager == null)
            return;

         // get a vector enumerator
         VectorEnumerator enumerator = vectorGameManager.GetVectorEnumerator();
         if (enumerator != null)
         {
            int index = 0;
            for (; ; )
            {
               // get the vector
               Vector vector = enumerator.GetNextVector();
               if (!vector.valid)
                  break;

               // switch to longer than 16 bits
               int startX = vector.startX;
               int startY = vector.startY;
               int endX = vector.endX;
               int endY = vector.endY;

               
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
               if (endX == startX && endY == startY)
               {
                  endX += strokeThickness / 2;
                  startX += strokeThickness / 2;
                  endY -= strokeThickness / 2;
                  startY += strokeThickness / 2;
               }

               line.Stroke = new SolidColorBrush(Color.FromArgb(255, vector.r, vector.g, vector.b));
               line.X1 = canvas.ActualWidth / 2 + startX;
               line.X2 = canvas.ActualWidth / 2 + endX;
               line.Y1 = canvas.ActualHeight / 2 - startY;
               line.Y2 = canvas.ActualHeight / 2 - endY;
               line.StrokeThickness = 100;
               line.Visibility = Visibility.Visible;

               ++index;
            }

            while (index < lines.Count)
               lines[index++].Visibility = Visibility.Collapsed;
         }
      }
   }
}

