using System;
using System.Collections.Generic;
using System.Linq;

namespace Grassman
{
    public sealed class TensorMatrix : AbstractTensorMatrix
    {
        public int L { get; private set; } //What I know I cannot say
        List<NumPow>[] matrixIndex = new List<NumPow>[2];
        /*Creates a generalized exterior algebra subspace matrix (Instead of for any v v^v = 0, v^...^v(l times) = 0)
         * based on M (square), which has the dimension of k*/
        public TensorMatrix(Matrix M, int k, int l)
        {
            if (constructorCheck(M, k) || k >= M.W * (l - 1) || l < 2)
                throw new Exception("Sample text");
        }
        //Ought to rewrite all of this
        void Loop(int n)
        {

            if (n != 0)
            {
                int i;
                if (matrixIndex[b] != null)
                    i = matrixIndex[b].Last().value + 1;
                else
                {
                    i = 0;
                    matrixIndex[b] = new List<NumPow>();
                }
                int j = matrixIndex[b].Count;
                matrixIndex[b].Add(new NumPow(i, 1));
                for (; i <= W - n; ++i)
                {
                    Loop(n - 1);
                    ++matrixIndex[b][j].value;
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
                //this[line, column] = Minor(K);
                if (b == 0)
                {
                    ++line;
                    column = 0;
                }
                else
                    ++column;
            }
        }
    }
}
