using System;
using System.IO;
using System.Net.Sockets;
using System.Diagnostics;

namespace TCPTest
{
    class Connection//for each connection
    {
        Matrix A, B, W, hisP, yourP;//stand for other user's encrypted matrix (aka hisPiece)
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
                    CreatePublicData(buf, 1);
                    RequestComparePublicData(buf);
                }
                else if (buf[0] == (byte)PackageCode.RequestComparePublicData)
                {
                    ComparePublicDataCallback(ComparePublicData(buf));
                }
                else if (buf[0] == (byte)PackageCode.PublicDataIsTheSame)
                {
                    samePublicData = true;
                    using (FileStream f = new FileStream("source.bin", FileMode.OpenOrCreate))
                    {
                        byte[] _buf = PublicDataToByteArray2();
                        f.Write(buf, 1, _buf.Length - 1);
                    }
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
            byte[] buf = PublicDataToByteArray();
            stream.Write(buf, 0, buf.Length);
        }
        byte[] PublicDataToByteArray()//server
        {
            //get matrices with the help of c++ project
            byte[] buf = new byte[packageSize];
            //Process.Start("Diffie-Hellman_based_Interpretation.exe", "s");
            //Process.Start("", "");
            Matrix.Server_GenerateSource();
            using (FileStream f = new FileStream("source.bin", FileMode.Open))
            {
                f.Read(buf, 0, packageSize);
            }
            CreatePublicData(buf, 0);
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
        void CreatePublicData(byte[] buf, int _offset)//client
        {
            int offset = _offset;
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
            byte[] _buf = new byte[packageSize];
            Matrix.MakePublic();
            using (FileStream f = new FileStream("public.bin", FileMode.Open))
            {
                f.Read(_buf, 0, packageSize);
            }
            int offset = 0;
            yourP = ByteArrayToMatrix(_buf, ref offset);
            byte[] buf = SendYourPiece2();
            stream.Write(buf, 0, buf.Length);
        }
        byte[] SendYourPiece2()
        {
            int size = 2 + sizeof(int) * W.data.Length;//let it be W
            byte[] buf = new byte[size];
            buf[0] = (byte)PackageCode.SendYourPiece;
            buf[1] = (byte)W.size;
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
            
            int size = sizeof(int) * hisP.data.Length + 1;
            byte[] buf = new byte[size];
            buf[0] = (byte)hisP.size;
            Buffer.BlockCopy(hisP.data, 0, buf, 1, size - 1);
            using (FileStream f = new FileStream("public.bin", FileMode.OpenOrCreate))
            {
                f.Write(buf, 0, size);
            }
            Matrix.MakePrivate();
            using (FileStream f = new FileStream("privatekey.bin", FileMode.Open))
            {
                f.Read(buf, 0, size);
            }
            int offset = 0;
            return ByteArrayToMatrix(buf, ref offset);
        }

    }
}