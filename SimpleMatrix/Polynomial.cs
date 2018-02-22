using System;
using System.Collections.Generic;
using System.Linq;
namespace SimpleMatrix
{
    class Polynomial
    {
        public List<double> Coeff { get; protected set; }
        public int Deg { get; protected set; }
        public Polynomial(params double[] list)//check for ending zeroes
        {
            Coeff = new List<double>(list);
            //here
            Deg = Coeff.Count - 1;
        }
        public Polynomial(IEnumerable<double> list)//check for ending zeroes
        {
            Coeff = new List<double>(list);
            //here
            Deg = Coeff.Count - 1;
        }
        public static Polynomial operator +(Polynomial a, Polynomial b)//check for ending zeroes
        {
            return new Polynomial(a.Coeff.Zip(b.Coeff, (x, y) => x + y).Concat(a.Deg > b.Deg ? a.Coeff.Skip(b.Deg) : b.Coeff.Skip(a.Deg)));
        }
        //is not needed now but may be useful for GCD
        public static Polynomial operator -(Polynomial a, Polynomial b)//check for ending zeroes
        {
            return new Polynomial(a.Coeff.Zip(b.Coeff, (x, y) => x - y).Concat(a.Deg > b.Deg ? a.Coeff.Skip(b.Deg) : b.Coeff.Skip(a.Deg)));
        }
        public static Polynomial operator *(Polynomial a, Polynomial b)
        {
            List<double> list = new List<double>(Math.Max(a.Deg + b.Deg + 1, 0));
            for (int i = 0; i < a.Deg; ++i)
                for (int j = 0; j < b.Deg; ++j)
                    list[i + j] = a[i] * b[j];
            return new Polynomial(list);
        }
        public double this[int i]
        {
            get { return Coeff[i]; }
            set { Coeff[i] = value; }
        }
        /*
        bool IsNotZero()
        {
            return Coeff.Exists(x => x != 0);
        }
        */
        public static Polynomial GCD(Polynomial a, Polynomial b)//write this
        {
            throw new NotImplementedException();
        }
    }
}
