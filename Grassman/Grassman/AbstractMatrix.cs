using System;

namespace Grassman
{
    public abstract class AbstractMatrix
    {
        public int W { get; protected internal set; } //Width
        public int H { get; protected internal set; } //Height
        static int maxwidth = 10, maxheight = 10;
        public static int MaxWidth //Maximal allowed width
        {
            get { return maxwidth; }
            set
            {
                if (maxwidth < 1)
                    throw new Exception("Sample Text");
                else
                    maxwidth = value;
            }
        }
        public static int MaxHeight //Maximal allowed height
        {
            get { return maxheight; }
            set
            {
                if (maxheight < 1)
                    throw new Exception("Sample Text");
                else
                    maxheight = value;
            }
        }
        protected internal int[,] array; //The matrix itself

        //Checks whether w and h can be the width and the height of the matrix
        protected static readonly Func<int, int, bool> InvalidRange = (w, h) => w <= 0 || w > MaxWidth || h <= 0 || h > MaxHeight;

        //Allows to address A.array[i, j] as A[i,j]
        public int this[int i, int j]
        {
            get { return array[i, j]; }
            protected internal set { array[i, j] = value; }
        }


    } //Common matrix class
}
