#include "IncompatibleMatricesException.hpp"
#include "OutOfBoundsException.hpp"
#include "NegativeIndexException.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <random>
class Matrix{

	public :

    Matrix();
	Matrix(std::string name, int n, int m) throw (NegativeIndexException);
    void random(long range, int nonInvertible, int allowNegative );
    void zeros();
    Matrix(Matrix const& mat);
    Matrix(int n) throw (NegativeIndexException);
	~Matrix();

	void setElement(int l, int c, long val) throw (OutOfBoundsException);
	void setElement(int i, long val) throw (OutOfBoundsException);
    void rank_1(long range);
    long getElement(int l, int c) const throw (OutOfBoundsException);
    long getElement(int i) const throw (OutOfBoundsException);
    int getSize() const;
    int getM() const;
    int getN() const;

    Matrix mult(Matrix const& mat) throw (IncompatibleMatricesException);
    int max_number_of_digits() const;
    friend std::ostream& operator<<(ostream& flux, Matrix const& mat);
	Matrix operator*(Matrix const& mat) throw (IncompatibleMatricesException);
    Matrix operator+(Matrix const& mat) throw (IncompatibleMatricesException);
    Matrix operator-(Matrix const& mat) throw (IncompatibleMatricesException);

    bool operator==(Matrix const& mat);
    bool operator!=(Matrix const& mat);

    Matrix& operator=(Matrix const& copy);

    void modulo(int mod);
    Matrix randomPolynomial(int coefMax) throw (IncompatibleMatricesException);

    static int nb_op;

	private :

	int n;      // matrix's number of rows
	int m;      // matrix's number of columns
	long* M;    // array representing the matrix
    string name;// matrix's name

};
