using System;
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
        RequestCompareHisPiece, YourPieceIsTheSame, YourPieceIsNotTheSame, HisPieceIsTheSame, HisPieceIsNotTheSame
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
        const int packageSize = 1024;//implying it will always be enough
        Matrix A, B, W, hisP, yourP;//F stand for other user's encrypted matrix (aka hisPiece)
        public int M { get; private set; }
        public int K { get; private set; }
        bool isServer;
        bool samePublicData = false, sameHisPiece = false, sameYourPiece = false;
        public NetworkStream stream;
        internal TCPModule module;
        internal Connection(TCPModule module, NetworkStream stream, bool isServer)
        {
            this.module = module;
            this.stream = stream;
            this.isServer = isServer;
        }
        public Matrix Protocol()
        {
            if (isServer)
                CreatePublicData();
            while (true)
            {
                byte[] buf = new byte[packageSize];
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

                }
                else if (buf[0] == (byte)PackageCode.YourPieceIsTheSame)
                {
                    sameYourPiece = true;
                    if (sameHisPiece)
                        CorrectFinish();
                }
                else if (buf[0] == (byte)PackageCode.HisPieceIsTheSame)
                {
                    sameHisPiece = true;
                    if (sameYourPiece)
                        CorrectFinish();                     
                }
                else
                {
                    //other cases left;
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
            return (buf.SequenceEqual(buf2));
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
                byte[] buf = PublicDataToByteArray();
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
            Matrix m = Matrix.CalculateYourPart(A, B, W, M, K);
            Buffer.BlockCopy(m.data, 0, buf, 2, sizeof(int) * m.data.Length);
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
            return (buf.SequenceEqual(buf2));
        } 
        void CompareYourPieceCallback(bool b)
        {
            if (b)
            {
                sameYourPiece = true;
                stream.Write(new byte[] { (byte)PackageCode.PublicDataIsTheSame }, 0, 1);
            }
            else
            {
                byte[] buf = PublicDataToByteArray();
                buf[0] = (byte)PackageCode.PublicDataIsNotTheSame;
                stream.Write(buf, 0, buf.Length);
            }
        }
        //the same for his piece
        Matrix CorrectFinish()//also would be incorrect
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
        const int maxconnections = 5;
        List<Connection> connections = new List<Connection>();//both as server and as client
        public void Connect(IPEndPoint IPEP)//client
        {
            if (connections.Count < maxconnections)
            {
                TcpClient client = new TcpClient(IPEP);
                client.Connect(IPEP);
                NetworkStream stream = client.GetStream();
                Connection con = new Connection(this, stream, false);
                connections.Add(con);
                con.Protocol();
                connections.Remove(con);
            }
        }
        void Accept(IPEndPoint IPEP)//server
        {
            if (connections.Count < maxconnections)
            {
                TcpListener server = new TcpListener(IPEP);
                server.Start();
                TcpClient client = server.AcceptTcpClient();
                NetworkStream stream = client.GetStream();
                server.Stop();
                Connection con = new Connection(this, stream, true);
                connections.Add(con);
                con.Protocol();//Just let the ke waste
                connections.Remove(con);
            } 
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                IPEndPoint IPEP = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 8888);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}