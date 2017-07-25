﻿using System;
using System.IO;
using System.Net.Sockets;
namespace TCPTest
{
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
            byte[] buf = new byte[packageSize];
            Matrix.Server_GenerateSource();
            using (FileStream f = new FileStream("public.bin", FileMode.Open))
            {
                f.Read(buf, 0, packageSize);
            }
            FileInfo fi = new FileInfo("public.bin");
            fi.Delete();
                CreatePublicData(buf);
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
            byte[] _buf = new byte[packageSize];
            Matrix.MakePublic();
            using (FileStream f = new FileStream("privatedata.bin", FileMode.Open))
            {
                f.Read(_buf, 0, packageSize);
            }
            int offset = 0;
            yourP = ByteArrayToMatrix(_buf, ref offset);
            Buffer.BlockCopy(yourP.data, 0, buf, 2, sizeof(int) * yourP.data.Length);
            stream.Write(buf, 0, buf.Length);
        }
        byte[] SendYourPiece2()
        {
            int size = 2 + sizeof(int) * W.data.Length;//let it be W
            byte[] buf = new byte[size];
            buf[0] = (byte)PackageCode.SendYourPiece;
            buf[1] = (byte)W.size;
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
            byte[] buf = new byte[packageSize];
            buf[0] = (byte)hisP.size;
            Buffer.BlockCopy(hisP.data, 0, buf, 0, sizeof(int) * hisP.data.Length);
            using (FileStream f = new FileStream("public.bin", FileMode.OpenOrCreate))
            {
                f.Write(buf, 0, hisP.data.Length + 1);
            }
            Matrix.MakePrivate();
            using (FileStream f = new FileStream("privatekey.bin", FileMode.Open))
            {
                f.Read(buf, 0, packageSize);
            }
            int offset = 0;
            return ByteArrayToMatrix(buf, ref offset);
        }
    }
}
