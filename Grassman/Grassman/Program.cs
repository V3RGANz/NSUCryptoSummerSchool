namespace Grassman //just integer arithmetic; don't care whether argument is out of range
{
    class NumPow
    {
        internal int value, power;
        internal NumPow(int value, int power)
        {
            this.value = value;
            this.power = power;
        }
    }

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