#include "Matrix.h"
#include <iostream>

using namespace mtrx;


AbstractMatrix::AbstractMatrix() { SetData({}); }
AbstractMatrix::AbstractMatrix(unsigned height, unsigned width) { resize(height, width); }
AbstractMatrix::AbstractMatrix(vector<vector<int>> Data) {
	SetData(Data);
}
AbstractMatrix::AbstractMatrix(unsigned size)
{
	vector<vector<int> > newData(size, vector<int>(h, 0));
	unsigned j = 0;
	for (unsigned i = 0; i < size; i++) {
		newData[i][j++] = 1;
	}
	SetData(newData);
}
AbstractMatrix::~AbstractMatrix() { data.clear(); }

unsigned AbstractMatrix::getH() {
	return h;
}
unsigned AbstractMatrix::getW() {
	return w;
}

void AbstractMatrix::setW(unsigned newW) {
	w = newW;
	data.resize(w);
}
void AbstractMatrix::setH(unsigned newH) {
	h = newH;
	for (vector<int>& line : data) {
		line.resize(h);
	}
}
void AbstractMatrix::resize(unsigned newH, unsigned newW) {
	setW(newW);
	setH(newH);
}
void AbstractMatrix::SetData(vector<vector<int>> newData) {
	data.clear();
	unsigned size = newData.size();
	setW(size);
	if (size) {
		data = newData;
		setH(data[0].size()); // expected that all columns aligned
	}
	else
		setH(0);
}

void AbstractMatrix::PrintMatrix() {
	for (std::vector<int> line : data) {
		for (int value : line)
			std::cout << value << "\t";
		std::cout << std::endl;
	}
}

vector<int>& AbstractMatrix::operator [](unsigned i) { return data[i]; }
AbstractMatrix AbstractMatrix::operator -() {
	AbstractMatrix result(data);
	for (vector<int>& line : result.data)
		for (int& value : line)
			value *= -1;
}

AbstractMatrix AbstractMatrix::operator +(AbstractMatrix A) throw (IncompatibleMatricesException) {
	
	if (h != A.getH() || w != A.getW())
		throw IncompatibleMatricesException();

	Matrix result;
	
	for (unsigned i = 0; i < w; i++){
		for (unsigned j = 0; j < h; j++)
			result[i][j] = (*this)[i][j] + A[i][j];
	}
	return result;
}

AbstractMatrix AbstractMatrix::operator -(AbstractMatrix A) throw (IncompatibleMatricesException) {

	if (h != A.getH() || w != A.getW())
		throw IncompatibleMatricesException();


	AbstractMatrix newA = -A;
	return *this + newA;
}

AbstractMatrix AbstractMatrix::operator *(AbstractMatrix A) throw (IncompatibleMatricesException) {
	// verification of the size of the two matrices
	if (w != A.getH())
		throw IncompatibleMatricesException();
	
	AbstractMatrix result(h, A.getW());

	for (unsigned i = 0; i < A.getW(); i++)
		for (unsigned j = 0; j < h; j++)
			for (unsigned k = 0; w; k++)
				result[i][j] += (*this)[i][k] * A[k][j];

	return result;
}
