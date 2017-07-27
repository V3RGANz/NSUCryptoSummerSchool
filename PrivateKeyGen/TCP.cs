using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;


namespace PrivateKeyGen
{
    enum PackageCode
    {
        CreatePublicData, RequestComparePublicData, PublicDataIsTheSame, PublicDataIsNotTheSame, SendYourPiece,
        RequestCompareHisPiece, YourPieceIsTheSame, YourPieceIsNotTheSame
    };

    class Matrix
    {
        [DllImport("Diffie-Hellman_based_Interpretation.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static void GeneratePublicData(int n, int k, int mod);
        [DllImport("Diffie-Hellman_based_Interpretation.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static void MakePublic();
        [DllImport("Diffie-Hellman_based_Interpretation.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static void MakePrivate(int mod);

        internal int[,] data { get; private set; }
        internal int size { get; private set; }//is excessive (the same goes for 'protected' modifier), but remains according to the c++ code. 
        internal Matrix(int[,] data, int size)
        {
            this.data = data;
            this.size = size;
        }
    }


    public class TCPModule
    {
        internal static int n, k, mod;
        //object _lock = new object(); 
        //const int maxconnections = 1; 
        //List<Connection> connections = new List<Connection>();//both as server and as client 
        //List<IPAddress> ips = new List<IPAddress>();//bound to 'connections' 
        internal static int[][,] Accept(int port, int n, int k, int mod)//server 
        {
            TCPModule.n = n;
            TCPModule.k = k;
            TCPModule.mod = mod;
            //if (connections.Count < maxconnections) 
            //{ 
            TcpClient client;
            TcpListener server;
            //lock (_lock) 
            //{ 
            server = new TcpListener(IPAddress.Any, 8888);
            server.Start();
            client = server.AcceptTcpClient();
            //} 
            IPAddress ip = ((IPEndPoint)client.Client.RemoteEndPoint).Address;
            //if (!ips.Any(x => x.Equals(ip))) 
            //{ 
            NetworkStream stream = client.GetStream();
            server.Stop();
            Connection con = new Connection(/*this, */stream, true);
            //connections.Add(con); 
            //ips.Add(ip); 
            var x = con.Protocol();
            //connections.Remove(con); 
            //ips.Remove(ip); 
            //} 
            client.Close();
            return x;
            //} 
        }
        internal static int[][,] Connect(string hostname, int port)//client 
        {
            TcpClient client;
            //if (connections.Count < maxconnections) 
            //{ 
            //lock (_lock) 
            //{ 
            client = new TcpClient();
            client.Connect(hostname, port);
            //} 
            IPAddress ip = ((IPEndPoint)client.Client.RemoteEndPoint).Address;
            //if (!ips.Any(x => x.Equals(ip))) 
            //{ 
            NetworkStream stream = client.GetStream();
            Connection con = new Connection(/*this, */stream, false);
            //connections.Add(con); 
            //ips.Add(ip); 
            var x = con.Protocol();
            //connections.Remove(con); 
            //ips.Remove(ip); 
            //} 
            client.Close();
            return x;
            //} 
        }
    }
}