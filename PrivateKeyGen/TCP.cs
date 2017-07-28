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
        internal extern static void GeneratePublicData(int n, int k, int mod, out IntPtr pd, out int size);
        [DllImport("Diffie-Hellman_based_Interpretation.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static void MakePublic(IntPtr publicdata, out IntPtr leftright, out IntPtr yourP);
        [DllImport("Diffie-Hellman_based_Interpretation.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static void MakePrivate(IntPtr leftright, IntPtr hisP, out IntPtr key);

        internal int[,] data { get; private set; }
        internal int size { get; private set; }//is excessive (the same goes for 'protected' modifier), but remains according to the c++ code.
        internal Matrix(int[,] data, int size)
        {
            this.data = data;
            this.size = size;
        }
    }

    class TCPModule
    {
        static TcpListener server;
        static TcpClient client;
        internal static TcpClient Accept(int port)//server 
        {
            server = new TcpListener(IPAddress.Any, 8888);
            server.Start();
            client = server.AcceptTcpClient();
            //server.BeginAcceptTcpClient(null, null);
            server.Stop();
            return client;
        }
        internal static TcpClient Connect(IPAddress ip, int port)//client  
        {
            client = new TcpClient();
            client.Connect(ip, port);
            //if(!client.BeginConnect(ip, port, null, null).AsyncWaitHandle.WaitOne(15000))
            return client;
        }
    }
}