using System;
using System.Collections.Generic;
using System.Linq;

namespace Grassman
{
    public sealed class GrassmanMatrix : AbstractTensorMatrix
    {
        List<int>[] matrixIndex = new List<int>[2]; /*Lists of ordered columns' and rows' numbers (see Loop, Minor)
        0 - line, 1 - column */

        // Creates an exterior algebra subspace matrix based on M (square), which has the dimension of k
        public GrassmanMatrix(Matrix M, int k)
        {
            if (constructorCheck(M, k) || k >= M.W)
                throw new Exception("Sample text");
            this.M = M;
            K = k;
            W = C(M.W, k);
            H = W;
            //L = l;
            array = new int[W, H];
            column = 0;
            line = 0;
            Loop(k);

            matrixIndex[0] = null;
            matrixIndex[1] = null;
            M = null;
            b = 0;
        }

        // n nested loops
        void Loop(int n)
        {
            if (n != 0)
            {
                int i;
                if (matrixIndex[b] != null)
                    i = matrixIndex[b].Last() + 1;
                else
                {
                    i = 0;
                    matrixIndex[b] = new List<int>();
                }
                int j = matrixIndex[b].Count;
                matrixIndex[b].Add(i);
                for (; i <= W - n; ++i)
                {
                    Loop(n - 1);
                    ++matrixIndex[b][j];
                }
                matrixIndex[b].Remove(matrixIndex[b].Last());
            }
            else
            {
                if (b == 0)
                {
                    b = 1;
                    Loop(K);
                    matrixIndex[b] = null;
                    b = 0;
                }
                else
                    this[line, column] = Minor(K);
                if (b == 0)
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
                return M[matrixIndex[0].Last(), matrixIndex[1].Last()];
            int sgn = 1, ret = 0, count = matrixIndex[1].Count;
            for (int i = 0; i < count; ++i)
            {
                int t = matrixIndex[1][i];
                matrixIndex[1].RemoveAt(i);
                ret += sgn * M[matrixIndex[0][K - j], t] * Minor(j - 1);
                matrixIndex[1].Insert(i, t);
                sgn *= -1;
            }
            return ret;
        }

        // Returns the number of k-combinations from n elements (not exactly, rather specified for this case)
        static int C(int n, int k)
        {
            int ret = 1;
            if (k > n >> 1)
                k = n - k;
            for (int i = 1; i <= k; ++i)
                ret = ret * (n - k + i) / i;
            return ret;
        }
    }
}
