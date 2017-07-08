using System;
using System.Collections.Generic;
using System.Linq;

namespace Grassman //just integer arithmetic; don't care whether argument is out of range
{
    abstract class AbstractMatrix
    {
        public int W { get; protected set; } //Width
        public int H { get; protected set; } //Height

        public const int MaxWidth = 10; //Maximal allowed width
        public const int MaxHeigth = 10; //Maximal allowed height

        protected int[,] array; //The matrix itself

        //Checks whether w and h can be the width and the height of the matrix
        protected bool InvalidRange(int w, int h)
        {
            return w <= 0 || w > MaxWidth || h <= 0 || h > MaxHeigth; 
        }

        //Allows to address A.array[i, j] as A[i,j]
        public int this[int i, int j]
        {
            get { return array[i, j]; }
            protected set { array[i, j] = value; }
        }


    } //Common matrix class

    class Matrix : AbstractMatrix
    {
        //Creates a w*h matrix, whose elements are random and belong to the [-minvalue, maxvalue] line segment
        public Matrix(int w, int h, int minvalue, int maxvalue)
        {
            if (InvalidRange(w, h))
                throw new Exception("Sample Text");
            Random r = new Random();
            W = w;
            H = h;
            array = new int[w, h];
            for (int i = 0; i < w; ++i)
                for (int j = 0; j < h; ++j)
                    this[i, j] = r.Next() % (maxvalue - minvalue + 1) - minvalue;
        }

        //Creates a w*h matrix, based on given 2-dimensional array
        public Matrix(int[,] array)
        {
            int w = array.GetLength(0), h = array.GetLength(1);
            if (InvalidRange(w,h))
                throw new Exception("Sample text");
            W = w;
            H = h;
            this.array = array;
        }

    } //General matrix class

    class GrassmanMatrix : AbstractMatrix
    {
        public int K { get; private set; } //Exterior algebra's subspace dimension
        int line = 0, column = 0; //Current matrix position (see Loop)
        List<int> lineIndex, columnIndex; //Lists of ordered columns' and rows' numbers (see Loop, Minor)
        bool loopState = true; //True stands for a loop by line, False - by column (see Loop)
        Matrix M; //Initial matrix

        // Creates an exterior algebra subspace matrix based on M, which has the dimension of k
        public GrassmanMatrix(Matrix M, int k)
        {
            if (M == null || k <= 0 || k >= M.W)
                throw new Exception("Sample text");
            this.M = M;
            K = k;
            W = C(M.W, k);
            H = M.W == M.H ? W : C(M.H, k);
            array = new int[W, H];
            lineIndex = new List<int>();
            Loop(k);

            column = 0;
            line = 0;
            lineIndex = null;
            columnIndex = null;
            loopState = true;
        }

        // Returns the number of k-combinations from n elements (not exactly, rather specified for this case)
        int C(int n, int k)
        {
            int ret = 1;
            if (k > n >> 1)
                k = n - k;
            for (int i = 1; i <= k; ++i)
                ret = ret * (n - k + i) / i;
            return ret;
        }

        // n nested loops
        void Loop(int n)
        {
            if (n != 0)
            {
                List<int> index = loopState ? lineIndex : columnIndex;
                int i = index.Any() ? index.Last() + 1 : 0;
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
                {
                    loopState = false;
                    columnIndex = new List<int>();//index == wlist
                    Loop(K);
                    loopState = true;
                }
                else
                    this[line, column] = Minor(K);
                if (loopState)
                {
                    ++line;
                    column = 0;
                }
                else
                    ++column;
            }
        }

        // j-th minor, whose lines and columns are defined by wlist and hlist
        int Minor(int j)
        {
            if (j == 1)
                return M[lineIndex.Last(), columnIndex.Last()];
            int sgn = 1, ret = 0, count = columnIndex.Count;
            for (int i = 0; i < count; ++i)
            {
                int t = columnIndex[i];
                columnIndex.RemoveAt(i);
                ret += sgn * M[lineIndex[K - j], t] * Minor(j - 1);
                columnIndex.Insert(i, t);
                sgn *= -1;
            }
            return ret;
        }
    } //Exterior algebra matrix

    class Program
    {
        static void Main(string[] args)
        {
            int[,] arr = { { 5, -2, 0 }, { 1, 3, 2 }, { 4, 1, 7 } };
            Matrix A = new Matrix(arr);
            GrassmanMatrix G = new GrassmanMatrix(A, 2);
        }
    }
}