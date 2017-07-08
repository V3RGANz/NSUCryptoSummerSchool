#pragma once
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <list>
using namespace std;

namespace Grassman {
	class AbstractMatrix {
	public:
		AbstractMatrix();
		AbstractMatrix(unsigned height, unsigned width);
		~AbstractMatrix();

		unsigned getH();
		unsigned getW();

		void setW(unsigned newW);
		void setH(unsigned newH);
		void resize(unsigned newH, unsigned newW);

		vector<int> operator [](unsigned i); // Allows use M[i][j] syntax

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
		/// <summary>
		/// Creates an exterior algebra subspace matrix based on M, which has the dimension of k
		/// </summary>
		/// <param name="M"></param>
		/// <param name="k"></param>
		GrassmanMatrix(Matrix M, unsigned k);
		GrassmanMatrix();
		~GrassmanMatrix();
	private:
		unsigned K;
		unsigned column = 0;
		unsigned line = 0;
		vector<unsigned> ColumnIndex, LineIndex;
		Matrix M;
		bool LoopState = true;

		/// <summary>
		/// Returns the number of combinations of k elements from n-elements set
		/// </summary>
		/// <param name="n"></param>
		/// <param name="k"></param>
		/// <returns></returns>
		unsigned C(unsigned n, unsigned k);
		/// <summary>
		/// search of all combination for columns or lines \
			passing through n nested loops, then call Minor for this\
			combination
		/// </summary>
		/// <param name="n"></param>
		void Loop(unsigned n);
		/// <summary>
		/// j-th minor, whose lines and columns are defined by LineIndex and ColumnIndex
		/// </summary>
		/// <param name="j"></param>
		/// <returns></returns>
		int Minor(unsigned j);
	}; // Exterior algebra matrix
}
