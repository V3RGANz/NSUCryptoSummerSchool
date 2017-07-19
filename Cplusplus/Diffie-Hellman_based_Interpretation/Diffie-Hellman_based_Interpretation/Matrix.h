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
		virtual AbstractMatrix operator -(AbstractMatrix);
		virtual AbstractMatrix operator *(AbstractMatrix);
		virtual AbstractMatrix operator *(int);
		virtual AbstractMatrix operator ^(unsigned);
		virtual AbstractMatrix operator -();
		virtual AbstractMatrix operator %(int);
		virtual bool operator ==(AbstractMatrix);

		//Prints all values in rectangular area
		void Print(ofstream&);

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
		Matrix(unsigned size, unsigned modulo);
		Matrix(Polynom p, Matrix A);
		~Matrix();
	}; //General matrix class

	class GrassmanExtendedMatrix : public AbstractMatrix { // not complete yet (нормально причесать а то наследуется дохрена мусора)
	public:
		GrassmanExtendedMatrix(vector<vector<int>>);
		~GrassmanExtendedMatrix();
		void printsource(ofstream&);
		void print(ofstream&);
	private:
		enum basisclassif { main, excess };
		unsigned N = 3, M = 3, K = 3, level = 1, depth = 0, index = 0;
		// current combination of index
		vector<unsigned> c;
		// permutation that will be dropped from combination
		vector<unsigned> exss;
		// all permutations
		vector<vector<unsigned>> permutations;
		// basis of extended matrix
		vector<vector<vector<unsigned> > > basis;
		// search all possible combinations for current N, M, K
		void CombTraverse();
		// all permutations of combination
		void PermutationTraverse();
		vector<vector<int>> source;
	}; // Matrix in extended grassman algebra space 

	class GrassmanMatrix : public AbstractMatrix {

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