#include "Matrix.h"
using namespace Grassman;

Matrix::Matrix() : AbstractMatrix() {}
Matrix::Matrix(unsigned height, unsigned width, int minvalue, int maxvalue) : AbstractMatrix(height, width) {
	for (unsigned i = 0; i < width; i++) {
		for (unsigned j = 0; j < width; j++) {
			(*this)[i][j] = minvalue + rand() % maxvalue;
		}
	}
}

Matrix::Matrix(vector<vector<int> > Data) : AbstractMatrix() {
	SetData(Data);
}

Matrix::~Matrix() {}