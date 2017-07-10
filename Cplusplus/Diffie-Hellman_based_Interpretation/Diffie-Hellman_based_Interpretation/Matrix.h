#ifndef MatrixHeader
#define MatrixHeader


#include <ctime>
#include <stdlib.h>
#include <vector>
#include "Polynom.h"

using namespace std;
namespace mtrx {


	// All matrices are squared

	class AbstractMatrix {
	public:
		//Create an empty matrix
		AbstractMatrix();
		//Create matrix filled by data Data
		AbstractMatrix(vector<vector<int> > Data);
		//Create size x size Identity matrix 
		AbstractMatrix(unsigned size);

		~AbstractMatrix();

		unsigned getSize();

		vector<vector<int> >getData();

		//Compute following polynom of current matrix
		AbstractMatrix computePoly(poly::Polynom A);

		/*
		Overloading 
		*/

		vector<int>& operator [](unsigned i); // Allows use M[i][j] syntax
		operator Matrix();
		AbstractMatrix operator +(AbstractMatrix);
		AbstractMatrix operator -(AbstractMatrix A);
		AbstractMatrix operator *(AbstractMatrix);
		AbstractMatrix operator *(int);
		AbstractMatrix operator ^(unsigned);
		AbstractMatrix operator -();
		bool operator ==(AbstractMatrix);
		
		//Prints all values in rectangular area
		void PrintMatrix();
	protected:
		void SetSize(unsigned size);
		// expected that newData represents sqare matrix 
		void SetData(vector<vector<int> > newData);
	
	private:
		unsigned size;
		vector<vector<int> > data;
	}; // Common matrix class

	class Matrix : public AbstractMatrix {
	public:
		Matrix();
		Matrix(unsigned size);
		Matrix(unsigned size, int minvalue, int maxvalue);
		Matrix(poly::Polynom p, Matrix A);
		Matrix(vector<vector<int> > Data);
		~Matrix();

	}; //General matrix class
	class GrassmanExtendedMatrix : public AbstractMatrix {
		GrassmanExtendedMatrix();
		GrassmanExtendedMatrix(vector<vector<int> >);
		~GrassmanExtendedMatrix();
	};
	class GrassmanMatrix : public GrassmanExtendedMatrix {
	public:
		// Creates an exterior algebra subspace matrix based on M, which has the dimension of k
		GrassmanMatrix(Matrix M, unsigned k);
		GrassmanMatrix();
		GrassmanMatrix(vector<vector<int> >);
		~GrassmanMatrix();
	private:
		unsigned K;
		unsigned column;
		unsigned line;
		vector<unsigned> ColumnIndex, LineIndex;
		Matrix M;
		bool LoopState = true;

		//Returns the number of combinations of k elements from n-elements set
		unsigned C(unsigned n, unsigned k);
		//	search of all combination for columns or lines 
		//	passing through n nested loops, then call Minor for this
		//	combination
		void Loop(unsigned n);
		//j-th minor, whose lines and columns are defined by LineIndex and ColumnIndex
		int Minor(unsigned j);
	}; // Exterior algebra matrix
}


#endif // MatrixHeader