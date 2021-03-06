﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
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
        TcpClient client;
        TcpListener server;
        bool isServer, running = false;
        string _k;
        //CancellationTokenSource cts = new CancellationTokenSource();
        bool Running
        {
            get { return running; }
            set
            {
                running = value;
                LaunchButton.Content = running ? "Стоп" : "Запуск";
                LaunchMenu.IsEnabled = !running;
                if (isServer)
                    server?.Stop();
                else
                    client?.Close();
            }
        }
        int result, port, n, k, mod;
        IPAddress ip;
        public MainWindow()
        {
            InitializeComponent();
        }

        private void ServerRB_Checked(object sender, RoutedEventArgs e)
        {
            isServer = true;
            ServerPanel.Visibility = Visibility.Visible;
            ClientPanel.Visibility = Visibility.Collapsed;
            LaunchButton.IsEnabled = true;
        }

        private void ClientRB_Checked(object sender, RoutedEventArgs e)
        {
            isServer = false;
            ServerPanel.Visibility = Visibility.Collapsed;
            ClientPanel.Visibility = Visibility.Visible;
            LaunchButton.IsEnabled = true;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!Running)
            {
                result = 0;
                if (isServer)
                {
                    IsIntTBCorrect(ServerPortTB, ref port, port => (port >= 1024 && port <= 65535));
                    IsIntTBCorrect(ServerNTB, ref n, N => (N >= 3 && N <= 15));
                    IsIntTBCorrect(ServerKTB, ref k, K => (K >= 1 && (result & 2) != 0));
                    IsIntTBCorrect(ServerModTB, ref mod, IsPrime);
                    if (result == 15)
                        Launch();
                }
                else
                {
                    IsIntTBCorrect(ClientPortTB, ref port, port => (port >= 1024 && port <= 65535));
                    if (IPAddress.TryParse(ClientIPTB.Text, out ip))
                        ++result;
                    else
                        ClientIPTB.Foreground = Brushes.Red;
                    if (result == 2)
                        Launch();
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
            if (k > n >> 1)
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

        private void MSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (MSlider.Value == 1)
            {
                _k = ServerKTB.Text;
                ServerKTB.Text = "3";
                ServerKTB.IsEnabled = false;
            }
            else
            {
                ServerKTB.IsEnabled = true;
                ServerKTB.Text = _k;
            }
                
            
        }

        private void TBGotFocus(object sender, RoutedEventArgs e)
        {
            (sender as TextBox).Foreground = Brushes.Black;
        }

        async void Launch()
        {
            Running = true;
            ErrorPanel.Visibility = Visibility.Collapsed;
            try
            {
                int[][,] m;
                //client = isServer ? await Task.Run(() => TCPModule.Accept(port))
                    //: await Task.Run(() => TCPModule.Connect(ip, port));
                if (isServer)
                {
                    server = new TcpListener(IPAddress.Any, 8888);
                    server.Start();
                    client = await server.AcceptTcpClientAsync();
                    server.Stop();
                }
                else
                {
                    client = new TcpClient();
                    await client.ConnectAsync(ip, port);
                }
                NetworkStream stream = client.GetStream();
                Connection con = new Connection(stream);
                ConnectedPanel.Visibility = Visibility.Visible;
                ConnectedLabel.Content = ((IPEndPoint)client.Client.RemoteEndPoint).Address;
                if (MSlider.Value == 1)
                    k = 0;
                m = isServer ? await Task.Run(() => con.Protocol(n, k, mod)) : await Task.Run(() => con.Protocol());
                KGPanel.Visibility = Visibility.Visible;
                WriteMatrix(A, m[0]);
                WriteMatrix(B, m[1]);
                WriteMatrix(W, m[2]);
                WriteMatrix(Key, m[3]);
            }
            catch (Exception ex)
            {
                ErrorTB.Text = ex.GetType() + ": " + ex.Message;
                ErrorPanel.Visibility = Visibility.Visible;
            }
            finally
            {
                Running = false;
                client?.Close();
            }
        }

        void WriteMatrix(UniformGrid M, int[,] data)
        {
            M.Children.Clear();
            M.Columns = data.GetLength(0);
            M.Rows = data.GetLength(1);
            for (int i = 0; i < M.Rows; ++i)
                for (int j = 0; j < M.Rows; ++j)
                    M.Children.Add(new TextBlock() { Text = data[i, j].ToString(), Margin = new Thickness(2), TextAlignment = TextAlignment.Center});
        }
    }
}
