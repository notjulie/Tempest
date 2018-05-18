using System.Windows;
using System.Windows.Controls;

using TempestWpf.Properties;

namespace TempestWpf
{
   /// <summary>
   /// Interaction logic for SettingsDialog.xaml
   /// </summary>
   public partial class SettingsDialog : Window
   {
      /// <summary>
      /// Initializes a new instance of class SettingsDialog
      /// </summary>
      public SettingsDialog()
      {
         // normal component initialization
         InitializeComponent();

         // add event handlers
         okButton.Click += OkButton_Click;

         // load settings
         string soundIOType = Settings.Default.SoundIOType;
         foreach (ComboBoxItem item in soundIOCombo.Items)
            if (item.Tag.ToString() == soundIOType)
               item.IsSelected = true;
         if (soundIOCombo.SelectedIndex < 0)
            soundIOCombo.SelectedIndex = 0;
      }

      private void OkButton_Click(object sender, RoutedEventArgs e)
      {
         // save the settings
         Settings.Default.SoundIOType = ((ComboBoxItem)soundIOCombo.SelectedItem).Tag.ToString();
         Settings.Default.Save();

         // close the dialog
         this.DialogResult = true;
      }
   }
}
