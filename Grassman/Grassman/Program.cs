using System;
using System.Collections.Generic;
using System.Linq;

namespace Grassman
<<<<<<< HEAD
{
    abstract class AbstractMatrix
    {
        public uint W { get; protected set; } //Width
        public uint H { get; protected set; } //Height

        protected int[,] array; //The matrix itself
                                /*
                                public AbstractMatrix(uint w, uint h)
                                {
                                    array = new int[w, h];
                                    W = w;
                                    H = h;
                                }
                                */

        public int this[int i, int j] //Allows to address A.array[i, j] as A[i,j]
        {
            get { return array[i, j]; }
            set { array[i, j] = value; }
        }

    } //Common matrix class

    class Matrix : AbstractMatrix
    {
        /// <summary>
        /// Creates a w*h matrix, whose elements are random and belong to the [-minvalue, maxvalue] line segment
        /// </summary>
        /// <param name="w"></param>
        /// <param name="h"></param>
        /// <param name="minvalue"></param>
        /// <param name="maxvalue"></param>
        public Matrix(uint w, uint h, int minvalue, int maxvalue)//потом будет по модулю p
        {
            Random r = new Random();
            W = w;
            H = h;
=======
{
    class AbstractMatrix
    {
        public uint W { get; protected set; } //Width
        public uint H { get; protected set; } //Height

        protected int[,] array; //The matrix itself
                                /*
                                public AbstractMatrix(uint w, uint h)
                                {
                                    array = new int[w, h];
                                    W = w;
                                    H = h;
                                }
                                */

        public int this[int i, int j] //Allows to address A.array[i, j] as A[i,j]
        {
            get { return array[i, j]; }
            set { array[i, j] = value; }
        }

    } //Common matrix class

    class Matrix : AbstractMatrix
    {
        /// <summary>
        /// Creates a w*h matrix, whose elements are random and belong to the [-minvalue, maxvalue] line segment
        /// </summary>
        /// <param name="w"></param>
        /// <param name="h"></param>
        /// <param name="minvalue"></param>
        /// <param name="maxvalue"></param>
        public Matrix(uint w, uint h, int minvalue, int maxvalue)//потом будет по модулю p
        {
            Random r = new Random();
>>>>>>> origin/master
            array = new int[w, h];
            for (int i = 0; i < w; ++i)
                for (int j = 0; j < h; ++j)
                    this[i, j] = r.Next() % (maxvalue - minvalue + 1) - minvalue;
        }
<<<<<<< HEAD

        /// <summary>
        /// Creates a w*h matrix, based on given 2-dimensional array
        /// </summary>
        /// <param name="w"></param>
        /// <param name="h"></param>
        /// <param name="array"></param>
        public Matrix(uint w, uint h, int[,] array)
        {
            W = w;
            H = h;
            this.array = array;
        }
        /*
        public Matrix(uint w, uint h)
        {
            array = new int[w, h];
            W = w;
            H = h;
        }
=======
        /*
        public Matrix(uint w, uint h)
        {
            array = new int[w, h];
            W = w;
            H = h;
        }
>>>>>>> origin/master
        */

    } //General matrix class

    class GrassmanMatrix : AbstractMatrix
    {
        public uint K { get; private set; } //Exterior algebra's subspace dimension
<<<<<<< HEAD
        int line = 0, column = 0; //Current matrix position (see Loop)
        List<uint> lineIndex, columnIndex; //Lists of ordered columns' and rows' numbers (see Loop, Minor)
        bool loopState = true; //True stands for a loop by line, False - by column (see Loop)
        Matrix M; //Initial

        /// <summary>
        /// Creates an exterior algebra subspace matrix based on M, which has the dimension of k
        /// </summary>
        /// <param name="M"></param>
        /// <param name="k"></param>
        public GrassmanMatrix(Matrix M, uint k)
        {
            this.M = M;
            K = k;
            W = C(M.W, k);
            H = M.W == M.H ? W : C(M.H, k);
            array = new int[W, H];
            lineIndex = new List<uint>();
            Loop(k);
            column = 0;
            line = 0;
            lineIndex = null;
            columnIndex = null;
            loopState = true;
        }

        /// <summary>
        /// Returns the number of k-combinations from n elements
        /// </summary>
        /// <param name="n"></param>
        /// <param name="k"></param>
=======
        int g_w = 0, g_h = 0; //Current matrix position (see Loop)
        List<uint> wlist, hlist; //Lists of ordered columns' and rows' numbers (see Loop, Minor)
        bool loopState; //True stands for a loop by line, False - by column (see Loop)
        Matrix M; //Initial

        /// <summary>
        /// Creates an exterior algebra subspace matrix based on M, which has the dimension of k
        /// </summary>
        /// <param name="M"></param>
        /// <param name="k"></param>
        public GrassmanMatrix(Matrix M, uint k)
        {
            this.M = M;
            K = k;
            W = C(M.W, k);
            H = M.W == M.H ? W : C(M.H, k);
            array = new int[W, H];
            wlist = new List<uint>();
            Loop(k);
        }

        /// <summary>
        /// Returns the number of k-combinations from n elements
        /// </summary>
        /// <param name="n"></param>
        /// <param name="k"></param>
>>>>>>> origin/master
        /// <returns></returns>
        uint C(uint n, uint k)//сделать
        {
            if (n < k)
                return 0;
            uint ret = 1;
            for (uint i = 1; i <= k; ++i)
                ret = ret * (n - k + i) / i;
            return ret;
        }


<<<<<<< HEAD
        /// <summary>
        /// n nested loops
        /// </summary>
=======
        /// <summary>
        /// n nested loops
        /// </summary>
>>>>>>> origin/master
        /// <param name="n"></param>
        void Loop(uint n)
        {
            if (n != 0)
            {
<<<<<<< HEAD
                List<uint> index = loopState ? lineIndex : columnIndex;
=======
                List<uint> index = loopState ? wlist : hlist;
>>>>>>> origin/master
                uint i = index.Any() ? index.Last() + 1 : 0;
                int j = index.Count;
                index.Add(i);
                for (; i <= W - n; ++i)
                {
                    Loop(n - 1);
                    ++index[j];
                }
                index.Remove(index.Last());
            }
            else
            {
                if (loopState)
<<<<<<< HEAD
                {
                    loopState = false;
                    columnIndex = new List<uint>();//index == wlist
                    Loop(K);
                    loopState = true;
                }
                else
                    this[line, column] = Minor(K);
                if (loopState)
                {
                    ++line;
                    column = 0;
=======
                {
                    loopState = false;
                    hlist = new List<uint>();//index == wlist
                    Loop(n);
                    loopState = true;
>>>>>>> origin/master
                }
                    
                else
<<<<<<< HEAD
                    ++column;
            }
        }

        /// <summary>
        /// j-th minor, whose lines and columns are defined by wlist and hlist
        /// </summary>
        /// <param name="j"></param>
=======
                    this[g_w, g_h] = Minor(K);
                if (loopState)
                    ++g_w;
                else
                    ++g_h;
            }
        }

        /// <summary>
        /// j-th minor, whose lines and columns are defined by wlist and hlist
        /// </summary>
        /// <param name="j"></param>
>>>>>>> origin/master
        /// <returns></returns>
        int Minor(uint j)
        {
            if (j == 1)
<<<<<<< HEAD
                return M[(int)lineIndex.Last(), (int)columnIndex.Last()];
            int sgn = 1, ret = 0, count = columnIndex.Count;
            for (int i = 0; i < count; ++i)
            {
                uint t = columnIndex[i];
                columnIndex.RemoveAt(i);
                ret += sgn * M[(int)lineIndex[(int)(K - j)], (int)t] * Minor(j - 1);
                columnIndex.Insert(i, t);
                sgn *= -1;
            }
            return ret;
=======
                return M[(int)wlist.First(), (int)hlist.First()];
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
>>>>>>> origin/master
        }
    } //Exterior algebra matrix

    class Program
    {
        static void Main(string[] args)
        {
<<<<<<< HEAD
            int[,] arr = { { 5, -2, 0 }, { 1, 3, 2 }, { 4, 1, 7 } };
            Matrix A = new Matrix(3, 3, arr);
            GrassmanMatrix G = new GrassmanMatrix(A, 2);
            Console.ReadKey();
=======
>>>>>>> origin/master
        }
    }
}