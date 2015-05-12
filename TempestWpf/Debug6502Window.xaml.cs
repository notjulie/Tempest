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
using System.Windows.Shapes;

namespace TempestWpf
{
   /// <summary>
   /// Interaction logic for Debug6502Window.xaml
   /// </summary>
   public partial class Debug6502Window : Window
   {
      /// <summary>
      /// Initializes a new instance of class Debug6502Window
      /// </summary>
      public Debug6502Window()
      {
         // normal initialization
         InitializeComponent();

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
         List<ListItem> items = new List<ListItem>();
         for (int i=0; i<disassembly.Count; ++i)
         {
            ListItem item = new ListItem();
            item.Text = disassembly[i];
            items.Add(item);
            /*DebugLineControl line = new DebugLineControl();
            line.Text = disassembly[i];
            listView.Items.Add(line);*/
         }

         listView.ItemsSource = items;
      }

      class ListItem
      {
         public string Text
         {
            get;
            set;
         }
      }
   }
}
