using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace PrivateKeyGen
{
    /// <summary> 
    /// Логика взаимодействия для MainWindow.xaml 
    /// </summary> 
    public partial class MainWindow : Window
    {
        bool server;
        bool running = false;
        bool Running
        {
            get { return running; }
            set
            {
                running = value;
                LaunchButton.Content = running ? "Стоп" : "Запуск";
            }
        }
        int result;
        int port, n, k, mod;
        string ip;
        public MainWindow()
        {
            InitializeComponent();
        }

        private void ServerRB_Checked(object sender, RoutedEventArgs e)
        {
            server = true;
            ServerPanel.Visibility = Visibility.Visible;
            ClientPanel.Visibility = Visibility.Collapsed;
            LaunchButton.IsEnabled = true;
        }

        private void ClientRB_Checked(object sender, RoutedEventArgs e)
        {
            server = false;
            ServerPanel.Visibility = Visibility.Collapsed;
            ClientPanel.Visibility = Visibility.Visible;
            LaunchButton.IsEnabled = true;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!Running)
            {
                result = 0;
                if (server)
                {
                    IsIntTBCorrect(ServerPortTB, ref port, port => (port >= 1024 && port <= 65535));
                    IsIntTBCorrect(ServerNTB, ref n, N => (N >= 3 && N <= 15));
                    IsIntTBCorrect(ServerKTB, ref k, K => (K >= 1 && (result & 2) != 0 && K < n));
                    IsIntTBCorrect(ServerModTB, ref mod, IsPrime);
                    if (result == 15)
                        Launch(true);
                }
                else
                {
                    IsIntTBCorrect(ClientPortTB, ref port, port => (port >= 1024 && port <= 65535));
                    try
                    {
                        IPAddress.Parse(ClientIPTB.Text);
                        ip = ClientIPTB.Text;
                        ++result;
                    }
                    catch
                    {
                        ClientIPTB.Foreground = Brushes.Red;
                    }
                    if (result == 2)
                        Launch(false);
                }
            }
            else
                Running = false;
        }

        bool IsPrime(int x)
        {
            if (x <= 1)
                return false;
            int y = (int)Math.Sqrt(x);
            for (int i = 2; i <= y; ++i)
                if (x % i == 0)
                    return false;
            return true;
        }

        /* 
        int C(int n, int k) 
        { 
        int ret = 1; 
        if (k > n » 1) 
        k = n - k; 
        for (int i = 0; i < k; ++i) 
        ret = ret * (n - k) / (k + 1); 
        return ret; 
        } 
        */

        void IsIntTBCorrect(TextBox tb, ref int t, Predicate<int> P)
        {
            result <<= 1;
            if (int.TryParse(tb.Text, out t) && P(t))
                ++result;
            else
                tb.Foreground = Brushes.Red;
        }

        private void TBGotFocus(object sender, RoutedEventArgs e)
        {
            (sender as TextBox).Foreground = Brushes.Black;
        }

        async void Launch(bool server)
        {
            Running = true;
            ErrorPanel.Visibility = Visibility.Collapsed;
            try
            {
                int[][,] m;
                if (server)
                    m = await Task.Run(() => TCPModule.Accept(port, n, k, mod));
                else
                    m = await Task.Run(() => TCPModule.Connect(ip, port));
                KGPanel.Visibility = Visibility.Visible;
                WriteMatrix(A, m[0]);
                WriteMatrix(B, m[1]);
                WriteMatrix(W, m[2]);
                WriteMatrix(Key, m[3]);
            }
            catch (Exception ex)
            {
                ErrorTB.Text = ex.Message;
                ErrorPanel.Visibility = Visibility.Visible;
            }
            finally
            {
                Running = false;
            }
        }

        void WriteMatrix(UniformGrid M, int[,] data)
        {
            M.Children.Clear();
            M.Columns = data.GetLength(0);
            M.Rows = data.GetLength(1);
            for (int i = 0; i < M.Rows; ++i)
                for (int j = 0; j < M.Rows; ++j)
                    M.Children.Add(new TextBlock() { Text = data[i, j].ToString(), Margin = new Thickness(2) });
        }
    }
}