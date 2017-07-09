#include "Matrix.h"
using namespace mtrx;

Matrix::Matrix() : AbstractMatrix() {}
Matrix::Matrix(unsigned height, unsigned width, int minvalue, int maxvalue) : AbstractMatrix(height, width) {
	for (unsigned i = 0; i < width; i++) {
		for (unsigned j = 0; j < width; j++) {
			(*this)[i][j] = rand() % (maxvalue - minvalue + 1);
		}
	}
}

Matrix::Matrix(vector<vector<int> > Data) : AbstractMatrix(Data) {}

Matrix::~Matrix() {}