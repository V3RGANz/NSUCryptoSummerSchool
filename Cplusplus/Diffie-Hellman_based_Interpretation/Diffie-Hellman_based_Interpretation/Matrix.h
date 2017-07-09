#pragma once
#include <ctime>
#include <stdlib.h>
#include <vector>
#include "incompatibleMatricesEcxeption.h"
using namespace std;
#include "Polynom.h"

namespace mtrx {

	class AbstractMatrix {
	public:

		/*
		Constructor, destructor
		*/

		//Create an empty matrix
		AbstractMatrix();
		//Create matrix with height x width size
		AbstractMatrix(unsigned height, unsigned width);
		//Create matrix filled by data Data
		AbstractMatrix(vector<vector<int> > Data);
		//Create size x size Identity matrix
		AbstractMatrix(unsigned size);
		~AbstractMatrix();

		unsigned getH();
		unsigned getW();

		void setW(unsigned newW);
		void setH(unsigned newH);
		void resize(unsigned newH, unsigned newW);
		//Compute following polynom of current matrix
		AbstractMatrix computePoly(poly::Polynom A) {

		}

		/*
		Overloading 
		*/

		vector<int>& operator [](unsigned i); // Allows use M[i][j] syntax

		///<exception cref="IncompatibleMatricesException.h">
		///in case if matrices has different size
		///</exception>
		AbstractMatrix operator +(AbstractMatrix) throw (IncompatibleMatricesException);
		AbstractMatrix operator -(AbstractMatrix A) throw(IncompatibleMatricesException);
		AbstractMatrix operator *(AbstractMatrix) throw (IncompatibleMatricesException);
		AbstractMatrix operator -();

		//Prints all values in rectangular area
		void PrintMatrix();
	protected:
		void SetData(vector<vector<int> > newData);
	
	private:
		unsigned h;
		unsigned w;
		vector<vector<int> > data;
	}; // Common matrix class

	class Matrix : public AbstractMatrix {
	public:
		Matrix();
		Matrix(unsigned height, unsigned width, int minvalue, int maxvalue);
		Matrix(vector<vector<int> > Data);
		~Matrix();

	}; //General matrix class

	class GrassmanMatrix : public AbstractMatrix {
	public:
		// Creates an exterior algebra subspace matrix based on M, which has the dimension of k
		GrassmanMatrix(Matrix M, unsigned k);
		GrassmanMatrix();
		GrassmanMatrix(vector<vector<int> >);
		~GrassmanMatrix();
	private:
		unsigned K;
		unsigned column = 0;
		unsigned line = 0;
		vector<unsigned> ColumnIndex, LineIndex;
		Matrix M;
		bool LoopState = true;

		//Returns the number of combinations of k elements from n-elements set
		unsigned C(unsigned n, unsigned k);
		// search of all combination for columns or lines \
			passing through n nested loops, then call Minor for this\
			combination
		void Loop(unsigned n);
		//j-th minor, whose lines and columns are defined by LineIndex and ColumnIndex
		int Minor(unsigned j);
	}; // Exterior algebra matrix
}
