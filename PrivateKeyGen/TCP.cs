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

    class TCPModule
    {
        internal static TcpClient Accept(int port)//server 
        {
            TcpClient client;
            TcpListener server;
            server = new TcpListener(IPAddress.Any, 8888);
            server.Start();
            client = server.AcceptTcpClient();
            server.Stop();
            return client;
        }
        internal static TcpClient Connect(IPAddress ip, int port)//client  
        {
            TcpClient client;
            client = new TcpClient();
            client.Connect(ip, port);
            //if(!client.BeginConnect(ip, port, null, null).AsyncWaitHandle.WaitOne(15000))
            return client;
        }
    }
}