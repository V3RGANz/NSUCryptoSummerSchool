using System;
using System.IO;
using System.Net.Sockets;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace PrivateKeyGen
{
    class Connection//for each connection
    {
        Matrix A, B, W, hisP, yourP;
        IntPtr pd, lr, yp, hp, key;
        internal int Mod { get; private set; }
        internal int K { get; private set; }
        bool samePublicData = false, sameYourPiece = false, sameHisPiece = false;
        const int packageSize = 4096;//implying it will always be enough
        public NetworkStream stream;
        internal Connection(NetworkStream stream)
        {
            this.stream = stream;
        }
        internal int[][,] Protocol(int n, int k, int mod)
        {
            int size;
            Matrix.GeneratePublicData(n, k, mod, out pd, out size);
            byte[] buf = new byte[size];
            Marshal.Copy(pd, buf, 0, size);
            CreatePublicData(buf, 0);
            byte[] buf2 = new byte[buf.Length + 1];
            Buffer.BlockCopy(buf, 0, buf2, 1, buf.Length);
            buf2[0] = (byte)PackageCode.CreatePublicData;
            stream.Write(buf2, 0, buf2.Length);
            return Protocol();
        }
        internal int[][,] Protocol()
        {
            byte[] buf = new byte[packageSize];   
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
        byte[] PublicDataToByteArray()//both
        {
            int size = 9 + sizeof(int) * (A.data.Length + B.data.Length + W.data.Length), offset = 1;
            byte[] buf = new byte[size];
            buf[0] = (byte)PackageCode.CreatePublicData;
            buf[offset++] = (byte)K;
            buf[offset] = (byte)Mod;
            offset += sizeof(int);
            PublicDataToByteArray2(A, buf, ref offset);
            PublicDataToByteArray2(B, buf, ref offset);
            PublicDataToByteArray2(W, buf, ref offset);
            return buf;
        }
        void PublicDataToByteArray2(Matrix M, byte[] buf, ref int offset)//both
        {
            buf[offset++] = (byte)M.size;
            Buffer.BlockCopy(M.data, 0, buf, offset, sizeof(int) * M.data.Length);
            offset += M.data.Length * sizeof(int);
        }
        void CreatePublicData(byte[] buf, int _offset)//client
        {
            int offset = _offset;
            K = buf[offset++];
            Mod = buf[offset];
            offset += sizeof(int);
            A = ByteArrayToMatrix(buf, ref offset);
            B = ByteArrayToMatrix(buf, ref offset);
            W = ByteArrayToMatrix(buf, ref offset);
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
            byte[] buf2 = PublicDataToByteArray();
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
                byte[] buf = PublicDataToByteArray();
                buf[0] = (byte)PackageCode.PublicDataIsNotTheSame;
                stream.Write(buf, 0, buf.Length);
            }
        }
        void SendYourPiece()//both here and later
        {
            int size = 1 + W.size * W.size * sizeof(int);
            byte[] _buf = new byte[size];
            Matrix.MakePublic(pd, out lr, out yp);
            Marshal.Copy(yp, _buf, 0, size);
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
        int[][,] CorrectFinish()
        {
            stream.Dispose();
            int size = sizeof(int) * hisP.data.Length + 1;
            byte[] buf = new byte[size];
            buf[0] = (byte)hisP.size;
            Buffer.BlockCopy(hisP.data, 0, buf, 1, size - 1);
            hp = Marshal.AllocHGlobal(size);
            Marshal.Copy(buf, 0, hp, size);
            Matrix.MakePrivate(lr, hp, out key);
            Marshal.Copy(key, buf, 0, size);
            int[][,] ret = new int[4][,];
            ret[0] = A.data;
            ret[1] = B.data;
            ret[2] = W.data;
            int offset = 0;
            ret[3] = ByteArrayToMatrix(buf, ref offset).data;
            return ret;
        }

    }
}