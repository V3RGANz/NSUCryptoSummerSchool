#include "Matrix.hpp"

#include <stdlib.h> // rand
#include <sstream> // convert int to string

using namespace std;

int Matrix::nb_op = 0;

// default constructor to create tables of Matrix
Matrix::Matrix()
{

}

// create a n*m zero Matrix
Matrix::Matrix(string name, int n, int m) throw (NegativeIndexException)
{
    if((n<1) || (m<1)) throw NegativeIndexException();

    M = new long[n*m];
    int size = n*m;
    for(int i=0; i<size; i++)
        M[i] = 0;

    this->n = n;
    this->m = m;
    this->name = name;
}

// choose values for Matrix, giving an rank(1) Matrix
void Matrix::rank_1(long range)
{
    M = new long[n*m];
    int size = n*m;

    // select the vector which will be the basis for vectors of matrix
    int vect[n];
    for(int i=0; i<n; i++) {
        vect[i] = rand()%(range/10);
        if (rand()%2 == 0)
            vect[i] = -vect[i];
    }

    // select a factor
    int fact;
    for(int i=0; i<n*m; i+=m) {
        fact = rand()%10;
        if (rand()%2 == 0)
            fact = -fact;

        for(int j=i; j<i+m; j++) {
            M[j] = vect[j-i]*fact;
        }
    }
}

// choose random values for the Matrix, in ]-range;range[
void Matrix::random(long range, int nonInvertible, int allowNegative)
{
    srand(time(NULL));
    std::vector<int> lastLine;
    lastLine.resize(getN());
    for(int i=0; i<this->getN(); i++){
        int sum = 0;
        for(int j=0; j<this->getM()-nonInvertible; j++){


            this->setElement(i,j,rand() % range);
            if ((rand()%2 == 0) && allowNegative) // select a sign for the value
                this->setElement(i,j, getElement(i,j)*(-1));

            sum += this->getElement(i,j);
        }
        lastLine[i] = sum;
    }
    for (int i =0; i<lastLine.size(); i++){
        cout<< lastLine[i]<<"  ";
    }
    if (nonInvertible){
        for(int i=0; i<this->getN(); i++){
            this->setElement(i,getM()-1,lastLine[i]%range);
        }
    }


}
// reset a Matrix to 0 values
void Matrix::zeros()
{
    int size = n*m;
    for(int i=0; i<size; i++)
        M[i] = 0;
}

// create a copy of the Matrix mat
// overload the copy constructor
Matrix::Matrix(Matrix const& copy)
{
    n = copy.n;
    m = copy.m;
    name = copy.name;
    int size = n*m;
    M = new long[size];
    for(int i=0; i<size; i++)
        M[i] = copy.M[i];
}

// create a n*n identity Matrix
Matrix::Matrix(int n) throw (NegativeIndexException)
{
    if (n<1) throw NegativeIndexException();

    this->n = n;
    m = n;
    this->name = name;
    int size = n*m;
    M = new long[size];
    for(int i=0; i<size; i++) {
        if((i%(n+1)) == 0)
            M[i] = 1;
        else M[i] = 0;
    }

    ostringstream number;
    number << n;
    this->name = "I" + number.str();
}

Matrix::~Matrix()
{
    delete []M;
}

void Matrix::setElement(int r, int c, long val) throw (OutOfBoundsException)
{
    if ((r > n) || (c > m))
        throw OutOfBoundsException();

    int index = r*m + c;
    M[index] = val;
}

void Matrix::setElement(int i, long val) throw (OutOfBoundsException)
{
    if (i >= n*m)
        throw OutOfBoundsException();

    M[i] = val;
}

long Matrix::getElement(int r, int c) const throw (OutOfBoundsException)
{
    if((r*m + c) >= n*m)
        throw OutOfBoundsException();

    return M[r*m + c];
}

long Matrix::getElement(int i) const throw (OutOfBoundsException)
{
    if(i >= n*m)
        throw OutOfBoundsException();

    return M[i];
}

// multiply the referred matrix by matrix in argument
// return the result of multiplication
Matrix Matrix::mult(Matrix const& mat) throw (IncompatibleMatricesException)
{
    // verification of the size of the two matrices
    if (m != mat.n)
        throw IncompatibleMatricesException();

    // declaration and initialisation of the resulting matrix
    int n_mult = n;
    int m_mult = mat.m;
    Matrix mult("tmp", n_mult, m_mult);

    // multiplication
    int i,j,k;
    for(i=0; i<n; i++)
        for(j=0; j<mat.m; j++)
            for(k=0; k<m; k++)
                mult.M[i*m_mult+j] += M[i*m+k] * mat.M[k*mat.m+j];

    return mult;
}

// return the number of digits for integer i
int number_of_digits(int i)
{
    int n = 1;
    int div;
    if(i<0)
        div = -10;
    else div = 10;

    while(i/div > 0) {
        n += 1;
        div *= 10;
    }

    if (i<0) return n+1;
    else return n;
}

// return the maximal number of digits of all values of the Matrix
int Matrix::max_number_of_digits() const
{
    int max_digits = 0;
    int size = n*m;
    for(int i=0; i<size; i++) {
        int num = number_of_digits(this->getElement(i));
        if (num > max_digits) max_digits = num;
    }
    return max_digits;
}

// overloading the * operator for 2 Matrices
// return m1*m2
Matrix Matrix::operator*(Matrix const& mat) throw (IncompatibleMatricesException)
{
    // verification of the size of the two matrices
    if (m != mat.n)
        throw IncompatibleMatricesException();

    // declaration and initialisation of the resulting matrix
    int n_mult = n;
    int m_mult = mat.m;
    Matrix mult("tmp", n_mult, m_mult);

    // multiplication
    int i,j,k;
    for(i=0; i<n; i++)
        for(j=0; j<mat.m; j++)
            for(k=0; k<m; k++)
                mult.M[i*m_mult+j] += M[i*m+k] * mat.M[k*mat.m+j];

    return mult;
}

// overloading the operator * for int*Matrix
Matrix operator*(int x, Matrix const& mat)
{
    Matrix res("tmp", mat.getN(), mat.getM());
    int i, j;
    int size = mat.getSize();
    for(i=0; i<size; i++) {
        res.setElement(i,x*mat.getElement(i));
        mat.nb_op++;
    }
    return res;
}

// overloading the operator * for Matrix*int
Matrix operator*(Matrix const& mat, int x)
{
    return x*mat;
}

// overloading the operator + for Matrix
Matrix Matrix::operator+(Matrix const& mat) throw (IncompatibleMatricesException)
{
    if((n != mat.n) || (m != mat.m))
        throw IncompatibleMatricesException();

    Matrix add("tmp", n, m);
    int size = n*m;
    for(int i=0; i<size; i++) {
        add.M[i] = M[i] + mat.M[i];
        nb_op++;
    }

    return add;
}

// overloading the operator - for Matrix
Matrix Matrix::operator-(Matrix const& mat) throw (IncompatibleMatricesException)
{
    if((n != mat.n) || (m != mat.m))
        throw IncompatibleMatricesException();

    Matrix add("tmp", n, m);
    int size = n*m;
    for(int i=0; i<size; i++) {
        add.M[i] = M[i] - mat.M[i];
        nb_op++;
    }

    return add;
}

// overloading the << operator for Matrix
// take the highest number of digits and print all values with
// enough space
ostream& operator<<(std::ostream &flux, Matrix const& mat)
{
    flux << mat.name << " :" << endl;

    int max_digits = mat.max_number_of_digits();
    int i,j;
    for(i=0; i<mat.n; i++) {
        flux << "| ";
        for(j=0; j<mat.m; j++) {
            int value = mat.getElement(i,j);
            int k = number_of_digits(value);
            while(k < max_digits) {
                k += 1;
                cout << " ";
            }
            flux << value << " ";
        }
        flux << " |" << endl;
    }
    return flux;
}

// overloading the == operator for Matrix
bool Matrix::operator==(Matrix const& mat)
{
    if((n != mat.n) || (m != mat.m))
        throw IncompatibleMatricesException();

    int i,j;
    int size = n*m;
    for(i=0; i<size; i++) {
        if(M[i] != mat.M[i])
            return false;
    }
    return true;
}

// overloading the != operator for Matrix
bool Matrix::operator!=(Matrix const& mat)
{
    return !((*this) == mat);
}

// overloading the operator =
Matrix& Matrix::operator=(Matrix const& copy)
{
    if(this != &copy) {
        n = copy.n;
        m = copy.m;

        M = NULL; // in case of non-allowed M table (default constructor)
        delete []M;

        int size = n*m;
        M = new long[size];
        for(int i=0; i<size; i++)
            M[i] = copy.M[i];
    }
    return (*this);
}

int Matrix::getSize() const
{
    return n*m;
}

int Matrix::getN() const
{
    return n;
}

int Matrix::getM() const
{
    return m;
}

void Matrix::modulo(int mod)
{
    int size = n*m;
    for(int i=0; i<size; i++) {
        if(M[i] >= mod)
            M[i] = M[i]%mod;
        else if (M[i] < 0)
            M[i] = mod-1-((-M[i]-1)%mod);
    }
}

// return a random polynomial from the Matrix selected,
// with factors in [-maxCoef; maxCoef]
Matrix Matrix::randomPolynomial(int maxCoef) throw (IncompatibleMatricesException)
{
    if (n != m) throw IncompatibleMatricesException();

    int coef = 0;
    Matrix polynomial("tmp", n, m);
    Matrix mult("tmp", n, m);
    for(int i=0; i<n; i++) {
        // select the coefficient for each block
        //while (coef == 0){
            coef = rand()%(maxCoef+1);//}

        if (rand()%2 == 0) coef = -coef;

        if(i==0) mult = Matrix(n);  // first coef is for identity matrix
        else {                      // compute A, A²,... , A^(n-1)

            mult = (*this);
            for(int j=0; j<i-1; j++)
                mult = mult*mult;
        }

        // add coef*mult to the resulting Matrix
        polynomial = polynomial + coef*mult;
    }
    return polynomial;
}

// AMELIORATIONS POSSIBLES:
//
//      -   empêcher le fait d'appeler une matrice I2, I3, ... IN
//      -   vérifier que le nom de la matrice est unique


