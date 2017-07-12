#pragma once
//#ifndef MatrixHeader
//#define MatrixHeader

#include <ctime>
#include <stdlib.h>
#include <vector>

using namespace std;

namespace protocol {
	// All matrices are squared

	class AbstractMatrix {
	public:
		//Create an empty matrix
		AbstractMatrix();
		//Create size x size Identity matrix 
		AbstractMatrix(unsigned size);

		~AbstractMatrix();

		unsigned getSize();
		vector<vector<int> > getData();

		vector<int>& operator [](unsigned i); // Allows use M[i][j] syntax
		
		operator Matrix (); // прямо говоря, это костыль

		virtual AbstractMatrix operator +(AbstractMatrix);
		virtual AbstractMatrix operator -(AbstractMatrix A);
		virtual AbstractMatrix operator *(AbstractMatrix);
		virtual AbstractMatrix operator *(int);
		virtual AbstractMatrix operator ^(unsigned);
		virtual AbstractMatrix operator -();
		virtual bool operator ==(AbstractMatrix);
		
		//Prints all values in rectangular area
		void Print();

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
		//Create matrix filled by data Data
		Matrix(vector<vector<int> > Data);
		Matrix(unsigned size);
		Matrix(unsigned size, int minvalue, int maxvalue);
		Matrix(Polynom p, Matrix A);
		~Matrix();

	}; //General matrix class

	class GrassmanExtendedMatrix : public AbstractMatrix {
	public:
		GrassmanExtendedMatrix();
		~GrassmanExtendedMatrix();
	}; //Extended Grassman algebra matrix class

	class GrassmanMatrix : public GrassmanExtendedMatrix {

	public:
		// Creates matrix, that represent matrix M in k-grade Grassman space
		GrassmanMatrix(Matrix M, unsigned k);
		~GrassmanMatrix();

	private:
		unsigned K;
		unsigned column;
		unsigned line;
		vector<unsigned> Index[2]; // Lines and Columns index combinations
		Matrix M;
		bool LoopState = true;
		//	search of all combination for columns or lines 
		//	passing through n nested loops, then call Minor for this
		//	combination
		void Loop(unsigned n);
		//j-th minor, whose lines and columns are defined by LineIndex and ColumnIndex
		int Minor(unsigned j);
	}; // Exterior algebra matrix
}


//#endif // MatrixHeader