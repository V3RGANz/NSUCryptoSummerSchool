#include "Header.h"
#include "Polynom.h"
#include "Matrix.h"

Matrix::Matrix() : AbstractMatrix() {}
Matrix::Matrix(unsigned size) : AbstractMatrix(size) { }
Matrix::Matrix(unsigned size, int minvalue, int maxvalue) { 
	// можно было бы сделать короче если было бы доступно поле data
	vector<vector<int> > newData(size, vector<int>(size));
	for (vector<int>& line : newData)
		for (int& value : line)
			value = minvalue + rand() % (maxvalue - minvalue + 1);
	SetData(newData);
}
Matrix::Matrix(Polynom p, Matrix A) { *this = p(A); }
Matrix::Matrix(vector<vector<int> > Data) : AbstractMatrix() { SetData(Data); }
Matrix::~Matrix() {}