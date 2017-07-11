#include "Matrix.h"
using namespace mtrx;

Matrix::Matrix() : AbstractMatrix() {}
Matrix::Matrix(unsigned size) : AbstractMatrix(size) { }
Matrix::Matrix(unsigned size, int minvalue, int maxvalue) { 
	// ����� ���� �� ������� ������ ���� ���� �� �������� ���� data
	vector<vector<int> > newData(size, vector<int>(size));
	for (vector<int>& line : newData)
		for (int& value : line)
			value = minvalue + rand() % (maxvalue - minvalue + 1);
	SetData(newData);
}
Matrix::Matrix(poly::Polynom p, Matrix A) { *this = p(A); }
Matrix::Matrix(vector<vector<int> > Data) : AbstractMatrix(Data) {}
Matrix::~Matrix() {}