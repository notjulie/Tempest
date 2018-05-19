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
         soundIOCombo.SelectionChanged += SoundIOCombo_SelectionChanged;

         // load settings
         string soundIOType = Settings.Default.SoundIOType;
         foreach (ComboBoxItem item in soundIOCombo.Items)
            if (item.Tag.ToString() == soundIOType)
               item.IsSelected = true;
         if (soundIOCombo.SelectedIndex < 0)
            soundIOCombo.SelectedIndex = 0;
         discoveryCOMPort.PortName = Settings.Default.DiscoveryCOMPort;
         loopbackPort1.PortName = Settings.Default.LoopbackPort1;
         loopbackPort2.PortName = Settings.Default.LoopbackPort2;

         // set initial state
         UpdateControlStates();
      }

      private void SoundIOCombo_SelectionChanged(object sender, SelectionChangedEventArgs e)
      {
         UpdateControlStates();
      }

      private void OkButton_Click(object sender, RoutedEventArgs e)
      {
         // save the settings
         Settings.Default.SoundIOType = ((ComboBoxItem)soundIOCombo.SelectedItem).Tag.ToString();
         if (Settings.Default.SoundIOType == "Discovery")
         {
            Settings.Default.DiscoveryCOMPort = discoveryCOMPort.PortName;
         }
         else if (Settings.Default.SoundIOType == "Loopback")
         {
            Settings.Default.LoopbackPort1 = loopbackPort1.PortName;
            Settings.Default.LoopbackPort2 = loopbackPort2.PortName;
         }

         Settings.Default.Save();

         // close the dialog
         this.DialogResult = true;
      }

      private void UpdateControlStates()
      {
         discoverySettings.Visibility = discoveryItem.IsSelected ? Visibility.Visible : Visibility.Collapsed;
         loopbackSettings.Visibility = loopbackItem.IsSelected ? Visibility.Visible : Visibility.Collapsed;
      }
   }
}
