using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;


namespace TCPTest
{
    enum PackageCode
    {
        CreatePublicData, RequestComparePublicData, PublicDataIsTheSame, PublicDataIsNotTheSame, SendYourPiece,
        RequestCompareHisPiece, YourPieceIsTheSame, YourPieceIsNotTheSame
    };

    class Matrix
    {

        [DllImport("Diffie-Hellman_based_Interpretation.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static void Server_GenerateSource();
        [DllImport("Diffie-Hellman_based_Interpretation.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static void MakePublic();
        [DllImport("Diffie-Hellman_based_Interpretation.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static void MakePrivate();

        public int[,] data { get; protected set; }
        public int size { get; protected set; }//is excessive (the same goes for 'protected' modifier), but remains according to the c++ code.
        public Matrix(int[,] data, int size)
        {
            this.data = data;
            this.size = size;
        }
    }


    public class TCPModule
    {
        public bool isActive { get; private set; } = true;
        object _lock = new object();
        const int maxconnections = 1;
        List<Connection> connections = new List<Connection>();//both as server and as client
        List<IPAddress> ips = new List<IPAddress>();//bound to 'connections'
        public void Accept(int port)//server 
        {
            //try
            //{
            if (connections.Count < maxconnections)
            {
                TcpClient client;
                TcpListener server;
                lock (_lock)
                {
                    server = new TcpListener(IPAddress.Any, 8888);
                    server.Start();
                    client = server.AcceptTcpClient();
                }
                IPAddress ip = ((IPEndPoint)client.Client.RemoteEndPoint).Address;
                if (!ips.Any(x => x.Equals(ip)))
                {
                    Console.WriteLine("Press SPACE to start key generation and anything else otherwise.\n");
                    if (Console.ReadKey().Key == ConsoleKey.Spacebar)
                    {
                        NetworkStream stream = client.GetStream();
                        server.Stop();
                        Connection con = new Connection(this, stream, true);
                        connections.Add(con);
                        ips.Add(ip);
                        con.Protocol();
                        connections.Remove(con);
                        ips.Remove(ip);
                    }
                }
                client.Close();
            }
            //}
            //catch (Exception e)
            //{
            //    Console.WriteLine(e.Message);
            //}
        }
        public void Connect(string hostname, int port)//client  
        {
            //try 
            //{ 
            TcpClient client;
            if (connections.Count < maxconnections)
            {
                lock (_lock)
                {
                    client = new TcpClient();
                    client.Connect(hostname, port);
                }
                IPAddress ip = ((IPEndPoint)client.Client.RemoteEndPoint).Address;
                if (!ips.Any(x => x.Equals(ip)))
                {
                    NetworkStream stream = client.GetStream();
                    Connection con = new Connection(this, stream, false);
                    connections.Add(con);
                    ips.Add(ip);
                    Matrix M = con.Protocol();
                    connections.Remove(con);
                    ips.Remove(ip);
                }
                client.Close();
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            new TCPModule().Connect("25.29.92.112", 8888);
        }
    }
}