using System;
using System.Collections.Generic;
using System.Linq;
namespace SimpleMatrix
{
    class Polynomial
    {
        public const int MinusInfinity = -1;
        public List<double> Coeff{ get; protected set; }
        public int Deg { get { return Coeff.Count - 1; } }

        public Polynomial()
        {
            Coeff = new List<double>();
        }
        public Polynomial(Polynomial p)
        {
            Coeff = new List<double>(p.Coeff);
        }
        public Polynomial(params double[] list)
        {
            int i;
            for (i = list.Length - 1; i >= 0 && list[i] == 0; --i) ;
            Coeff = list.Take(i + 1).ToList();
        }
        public Polynomial(List<double> list)
        {
            Coeff = ZeroCheck(list);
        }
        public static Polynomial operator +(Polynomial a, Polynomial b)//check for ending zeroes
        {
            List<double> list = a.Coeff.Zip(b.Coeff, (x, y) => x + y).ToList();
            if (a.Deg == b.Deg)
            {
                list = ZeroCheck(list);
                return new Polynomial(list);
            }
            return new Polynomial(list.Concat(a.Deg > b.Deg ? a.Coeff.Skip(b.Deg + 1) : b.Coeff.Skip(a.Deg + 1)).ToList());
        }
        public static Polynomial operator *(Polynomial a, Polynomial b)
        {
            List<double> list = new List<double>(new double[Math.Max(a.Deg + b.Deg + 1, 0)]);
            for (int i = 0; i <= a.Deg; ++i)
                for (int j = 0; j <= b.Deg; ++j)
                    list[i + j] = a[i] * b[j];
            return new Polynomial(list);
        }
        public static Polynomial operator *(Polynomial p, double d)
        {
            if (d == 0)
                return new Polynomial();
            return new Polynomial(p.Coeff.Select(x => x * d).ToList());
        }
        public double this[int i]
        {
            get { return Coeff[i]; }
            set { Coeff[i] = value; }
        }
        static List<double> ZeroCheck(List<double> list)
        {
            int i;
            for (i = list.Count - 1; i >= 0 && list[i] == 0; --i) ;
            return list.Take(i + 1).ToList();
        }
        public static Polynomial GCD(Polynomial a, Polynomial b)
        {
            if (a.Deg <= MinusInfinity || b.Deg == MinusInfinity)
                return b;//not true btw but whatever
            return a.Deg < b.Deg ? GCD(a, GCD2(a, b)) : GCD(b, GCD2(b, a));
        }
        static Polynomial GCD2(Polynomial minor, Polynomial major)
        {
            Polynomial t = new Polynomial(minor);
            t.Coeff = Enumerable.Repeat(0d, major.Deg - minor.Deg).Concat(t.Coeff).ToList();
            return major + t * (-major[major.Deg] / t[t.Deg]);
        }
        public override string ToString()
        {
            if (Deg == -1)
                return "0";
            string ret = this[0].ToString();
            if (Deg == 0)
                return ret;
            ret += this[1] == 0 ? "" : " + " + this[1] + "x";
            for (int i = 2; i <= Deg; ++i)
                ret += this[i] == 0 ? "" : " + " + this[i] + "x^" + i;
            return ret;
        }
    }
}