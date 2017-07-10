#include "Matrix.h"
using namespace mtrx;


Matrix::Matrix() : AbstractMatrix() {}
Matrix::Matrix(unsigned size) : AbstractMatrix(size) { }
Matrix::Matrix(unsigned size, int minvalue, int maxvalue) {
	vector<vector<int> > newData(size, vector<int>(size));
	for (vector<int>& line : newData)
		for (int& value : line)
			value = rand() % (maxvalue - minvalue + 1);
	SetData(newData);
}
mtrx::Matrix::Matrix(poly::Polynom p, Matrix A) { SetData((A.computePoly(p)).getData()); }
Matrix::Matrix(vector<vector<int> > Data) : AbstractMatrix(Data) {}

Matrix::~Matrix() {}