using System;
using System.Collections.Generic;
using System.Linq;

namespace SimpleMatrix
{
    struct Permutation
    {
        public List<int> seq { get; private set; }
        public int sign { get; private set; }
        public int size { get; }
        public Permutation(IEnumerable<int> list)
        {
            seq = new List<int>(list);
            sign = 1;
            size = seq.Count;
        }
        public void Swap(int i, int j)
        {
            int t = seq[i];
            seq[i] = seq[j];
            seq[j] = t;
            sign *= -1;
        }
    }
    class Program
    {
        static Random r = new Random();
        static int size;
        static Polynomial[,] M;//square only
        static Permutation perm;
        static Stack<int> stack = new Stack<int>();
        static void GenerateCharacteristicMatrix(int size)
        {
            Program.size = size;
            M = new Polynomial[size, size];
            for (int i = 0; i < size; ++i)
                for (int j = 0; i < size; ++j)
                {
                    double t = r.NextDouble();
                    M[i, j] = i == j ? new Polynomial(t, -1) : new Polynomial(t);
                }   
        }
        static IEnumerable<Permutation> Permutations(int depth = 0)
        {
            if (depth == perm.size - 1)
                yield return perm;
            else
                for (int i = depth + 1; i < perm.size; ++i)
                {
                    foreach (Permutation x in Permutations(depth + 1))
                        yield return x;
                    perm.Swap(depth, i);
                }
        }
        static Polynomial Minor(int i, int j)
        {
            int[] li = (int[])Enumerable.Range(0, i).Concat(Enumerable.Range(i + 1, size - i - 1));
            perm = new Permutation(Enumerable.Range(0, j).Concat(Enumerable.Range(j + 1, size - j - 1)));
            Polynomial ret = new Polynomial(0);
            foreach (Permutation p in Permutations())
            {
                Polynomial mul = new Polynomial(p.sign);
                for (int k = 0; k < size; ++k)
                    mul *= M[li[k], p.seq[k]];
                ret += mul;
            }
            return ret;
        }
        static bool ToChtoNado()
        {
            int j = 1;
            Polynomial gcd = Minor(0, 0);
            for (int i = 0; i < size; ++i)
            {
                for (; j < size; ++j)
                {
                    gcd = Polynomial.GCD(gcd, Minor(i, j));
                    if (gcd.Deg <= 0)
                        return false;
                }
                j = 0;
            }
            return true; 
        }
        static void Main(string[] args)
        {
            GenerateCharacteristicMatrix(5);
            Console.WriteLine(ToChtoNado());
        }
    }
}