using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Grassman
{
    class Matrix
    {
        public Matrix G;
        public uint w, h;
        public int[,] value;
        uint c, g_w = 0, g_h = 0, k;
        List<uint> wlist, hlist;
        bool b = true;//xuynya

        public Matrix(uint w, uint h, int minvalue, int maxvalue)//potom budet modulo p
        {
            Random r = new Random();
            value = new int[w, h];
            for (int i = 0; i < w; ++i)
                for (int j = 0; j < h; ++j)
                    value[i, j] = r.Next() % (maxvalue - minvalue + 1) - minvalue;
        }

        Matrix(uint w, uint h)
        {
            value = new int[w, h];
        }

        Matrix(uint w, uint h, uint k, Matrix M)
        { }

        public Matrix BuildGrassman(uint k)
        {
            this.k = k;
            G = new Matrix(c, c);
            wlist = new List<uint>();
            Cycle(k, wlist, ref g_w);
            return G;
        }
    }

    class GrassmanMatrix : Matrix
    {
        uint c, g_w = 0, g_h = 0, k;
        List<uint> wlist, hlist;
        Matrix M;

        public GrassmanMatrix(uint w, uint h, int minvalue, int maxvalue) : base(w, h, minvalue, maxvalue) { }

        protected GrassmanMatrix()
        { }
        
        protected void Constructor(uint w, uint h, uint k, Matrix M)
        {
            this.M = M;
            this.k = k;
            this.w = w;
            this.h = h;
        }

        uint F(uint n)//xuynya
        {
            uint ret = 1;
            for (uint i = 1; i <= n; ++i)
                ret *= i;
            return ret;
        }

        uint C(uint n, uint k)//xuynya
        {
            return F(n) / (F(n - k) * F(k));
        }

        void Cycle(uint level, List<uint> index, ref uint gindex)
        {
            if (level != 0)
            {
                uint i;
                uint? t = index.Last();
                i = t == null ? 0 : index.Last() + 1;
                int j = index.Count;
                index.Add(i);
                for (; i <= c - level; ++i)
                {
                    Cycle(level - 1, index, ref gindex);
                    ++index[j];
                }
                index.Remove(index.Last());
            }
            else
            {
                if (b)
                {
                    b = false;
                    hlist = new List<uint>();//index == wlist
                    Cycle(k, hlist, ref g_h);
                    b = true;
                }
                else
                    G.value[g_w, g_h] = Minor(k);
                ++gindex;
            }
        }

        int Minor(uint j)
        {
            if (j == 1)
            {
                return value[wlist.First(), hlist.First()];
            }
            else
            {
                int sgn = 1, ret = 0;
                for (int i = 0; i < wlist.Count; ++i)
                {
                    uint h_t = hlist[i];
                    hlist.RemoveAt(i);
                    ret += sgn * (int)wlist[i] * Minor(j - 1);
                    hlist.Insert(i, h_t);
                    sgn *= -1;
                }
                return ret;
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Matrix A = new Matrix(3, 3, -100, 100);

        }
    }
}
