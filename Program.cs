﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Net;
using System.Net.Sockets;

namespace TCPTest
{
    enum PackageCode
    {
        CreatePublicData, RequestComparePublicData, PublicDataIsTheSame, PublicDataIsNotTheSame, SendYourPiece,
        RequestCompareHisPiece, YourPieceIsTheSame, YourPieceIsNotTheSame
    };

    class Matrix
    {
        public int[,] data { get; protected set; }
        public int size { get; protected set; }//is excessive (the same goes for 'protected' modifier), but remains according to the c++ code.
        public Matrix(int[,] data, int size)
        {
            this.data = data;
            this.size = size;
        }
        public static Matrix CalculateYourPart(Matrix A, Matrix B, Matrix W, int M, int K)
        {
            //Calculations are done in the c++ project.
            return (Matrix)A.MemberwiseClone();
        }
        public static Matrix GetKey(Matrix left, Matrix right, Matrix W)
        {
            //Calculations are done in the c++ project.
            return (Matrix)W.MemberwiseClone();
        }
    }

    class Connection//for each connection
    {
        Matrix A, B, W, hisP, yourP;//F stand for other user's encrypted matrix (aka hisPiece)
        public int M { get; private set; }
        public int K { get; private set; }
        bool isServer;
        bool samePublicData = false, sameYourPiece = false, sameHisPiece = false;
        const int packageSize = 1024;//implying it will always be enough
        public NetworkStream stream;
        internal TCPModule module;
        internal Connection(TCPModule module, NetworkStream stream, bool isServer)
        {
            this.module = module;
            this.stream = stream;
            this.isServer = isServer;
        }
        public Connection()
        {
        }
        public Matrix Protocol()
        {
            byte[] buf = new byte[packageSize];
            if (isServer)
                CreatePublicData();
            while (true)
            {
                stream.Read(buf, 0, packageSize);
                if (!samePublicData &&
                    (buf[0] == (byte)PackageCode.CreatePublicData || buf[0] == (byte)PackageCode.PublicDataIsNotTheSame))
                {
                    CreatePublicData(buf);
                    RequestComparePublicData(buf);
                }
                else if (buf[0] == (byte)PackageCode.RequestComparePublicData)
                {
                    ComparePublicDataCallback(ComparePublicData(buf));
                }
                else if (buf[0] == (byte)PackageCode.PublicDataIsTheSame)
                {
                    samePublicData = true;
                    SendYourPiece();
                }
                else if (buf[0] == (byte)PackageCode.SendYourPiece)
                {
                    ReceiveHisPiece(buf);
                    RequestCompareHisPiece(buf);
                }
                else if (buf[0] == (byte)PackageCode.RequestCompareHisPiece)
                {
                    CompareYourPieceCallback(CompareYourPiece(buf));
                    if (samePublicData && sameHisPiece && sameYourPiece)
                        return CorrectFinish();
                }
                else if (buf[0] == (byte)PackageCode.YourPieceIsTheSame)
                {
                       sameHisPiece = true;
                    if (samePublicData && sameYourPiece)
                        return CorrectFinish();
                    SendYourPiece();
                }
            }
        }
        void CreatePublicData()//server
        {
            if (isServer)
            {
                byte[] buf = PublicDataToByteArray();
                stream.Write(buf, 0, buf.Length);
            }
        }
        byte[] PublicDataToByteArray()//server
        {
            //get matrices with the help of c++ project
            A = new Matrix(new int[,] { { 1 } }, 1);
            B = new Matrix(new int[,] { { 1 } }, 1);
            W = new Matrix(new int[,] { { 1 } }, 1);
            M = 2;
            K = 2;
            return PublicDataToByteArray2();
        }
        byte[] PublicDataToByteArray2()//both
        {
            int size = 6 + sizeof(int) * (A.data.Length + B.data.Length + W.data.Length), offset = 1;
            byte[] buf = new byte[size];
            buf[0] = (byte)PackageCode.CreatePublicData;
            PublicDataToByteArray3(A, buf, ref offset);
            PublicDataToByteArray3(B, buf, ref offset);
            PublicDataToByteArray3(W, buf, ref offset);
            buf[offset++] = (byte)M;
            buf[offset] = (byte)K;
            return buf;
        }
        void PublicDataToByteArray3(Matrix M, byte[] buf, ref int offset)//both
        {
            buf[offset++] = (byte)M.size;
            Buffer.BlockCopy(M.data, 0, buf, offset, sizeof(int) * M.data.Length);
            offset += M.data.Length * sizeof(int);
        }
        void CreatePublicData(byte[] buf)//client
        {
            int offset = 1;
            A = ByteArrayToMatrix(buf, ref offset);
            B = ByteArrayToMatrix(buf, ref offset);
            W = ByteArrayToMatrix(buf, ref offset);
            M = buf[offset++];
            K = buf[offset];
        }
        Matrix ByteArrayToMatrix(byte[] buf, ref int offset)//client
        {
            int size = buf[offset++];
            int[,] data = new int[size, size];
            /*
            for (int i = 0; i < data.Length; ++i)
                for (int j = 0; j < data.Length; ++j)
                {
                    data[i, j] = BitConverter.ToInt32(buf, offset);
                    offset += sizeof(int);
                }
            */
            int count = sizeof(int) * data.Length;
            Buffer.BlockCopy(buf, offset, data, 0, count);
            offset += count;
            return new Matrix(data, size);
        }
        void RequestComparePublicData(byte[] buf)//client 
        {
            buf[0] = (byte)PackageCode.RequestComparePublicData;
            stream.Write(buf, 0, buf.Length);
        }
        bool ComparePublicData(byte[] buf)//server
        {
            byte[] buf2 = PublicDataToByteArray2();
            buf2[0] = (byte)PackageCode.RequestComparePublicData;
            for (int i = 0; i < buf2.Length; ++i)
                if (buf[i] != buf2[i])
                    return false;
            return true;
        }
        void ComparePublicDataCallback(bool b)//server
        {
            if (b)
            {
                samePublicData = true;
                stream.Write(new byte[] { (byte)PackageCode.PublicDataIsTheSame }, 0, 1);
            }
            else
            {
                byte[] buf = PublicDataToByteArray2();
                buf[0] = (byte)PackageCode.PublicDataIsNotTheSame;
                stream.Write(buf, 0, buf.Length);
            }
        }
        void SendYourPiece()//both here and later
        {
            byte[] buf = SendYourPiece2();
            stream.Write(buf, 0, buf.Length);
        }
        byte[] SendYourPiece2()
        {
            int size = 2 + sizeof(int) * W.data.Length;//let it be W
            byte[] buf = new byte[size];
            buf[0] = (byte)PackageCode.SendYourPiece;
            buf[1] = (byte)W.size;
            yourP = Matrix.CalculateYourPart(A, B, W, M, K);
            Buffer.BlockCopy(yourP.data, 0, buf, 2, sizeof(int) * yourP.data.Length);
            return buf;
        }
        void ReceiveHisPiece(byte[] buf)
        {
            int offset = 1;
            hisP = ByteArrayToMatrix(buf, ref offset);
        }
        void RequestCompareHisPiece(byte[] buf)
        {
            buf[0] = (byte)PackageCode.RequestCompareHisPiece;
            stream.Write(buf, 0, buf.Length);
        }
        bool CompareYourPiece(byte[] buf)
        {
            byte[] buf2 = SendYourPiece2();
            buf2[0] = (byte)PackageCode.RequestCompareHisPiece;
            for (int i = 0; i < buf2.Length; ++i)
                if (buf[i] != buf2[i])
                    return false;
            return true;
        }
        void CompareYourPieceCallback(bool b)
        {
            if (b)
            {
                sameYourPiece = true;
                stream.Write(new byte[] { (byte)PackageCode.YourPieceIsTheSame }, 0, 1);
            }
            else
            {
                byte[] buf = SendYourPiece2();
                buf[0] = (byte)PackageCode.YourPieceIsNotTheSame;
                stream.Write(buf, 0, buf.Length);
            }
        }
        Matrix CorrectFinish()
        {
            stream.Dispose();
            if (isServer)
                return Matrix.GetKey(yourP, hisP, W);
            else
                return Matrix.GetKey(hisP, yourP, W);
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
                    Console.WriteLine("You've successfully been connected\nPress SPACE to start key generation and anything else otherwise.\n");
                    if (Console.ReadKey().Key == ConsoleKey.Spacebar)
                    {
                        NetworkStream stream = client.GetStream();
                        Connection con = new Connection(this, stream, false);
                        connections.Add(con);
                        ips.Add(ip);
                        con.Protocol();
                        connections.Remove(con);
                        ips.Remove(ip);
                    }
                }
                client.Close();
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            //try
            //{
            new TCPModule().Connect("172.16.15.134", 8888);
            //}
            //catch (Exception e)
            //{
            //Console.WriteLine(e.Message);
            //}
        }
    }
}