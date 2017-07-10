using System;

namespace Grassman
{
    public class Matrix : AbstractMatrix
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
                    this[i, j] = r.Next() % (maxvalue - minvalue + 1) + minvalue;
        }

        //Creates a w*h matrix, based on given 2-dimensional array
        public Matrix(int[,] array)
        {
            int w = array.GetLength(0), h = array.GetLength(1);
            if (InvalidRange(w, h))
                throw new Exception("Sample text");
            W = w;
            H = h;
            this.array = array;
        }

    } //General matrix class
}
