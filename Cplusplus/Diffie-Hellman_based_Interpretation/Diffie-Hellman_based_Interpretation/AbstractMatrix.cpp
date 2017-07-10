#include "Matrix.h"
#include "incompatibleMatricesException.h"
#include <iostream>

using namespace mtrx;


AbstractMatrix::AbstractMatrix() { SetData({}); }
//AbstractMatrix::AbstractMatrix(unsigned height, unsigned width) { resize(height, width); }
AbstractMatrix::AbstractMatrix(vector<vector<int>> Data) {
	SetData(Data);
}
AbstractMatrix::AbstractMatrix(unsigned size)
{
	vector<vector<int> > newData(size, vector<int>(size, 0));
	unsigned j = 0;
	for (unsigned i = 0; i < size; i++) {
		newData[i][j++] = 1;
	}
	SetData(newData);
}

AbstractMatrix::~AbstractMatrix() { /*data.clear(); */}

unsigned mtrx::AbstractMatrix::getSize() { return size; }

vector<vector<int>> mtrx::AbstractMatrix::getData() { return data;}

AbstractMatrix mtrx::AbstractMatrix::computePoly(poly::Polynom A) // !!! to be done 
{
	AbstractMatrix ret(vector<vector<int> >(size, vector<int>(size, 0)));
	for (unsigned int i = 0; i < A.getDegree(); i++) {
		ret = ret + ((*this) ^ i)*A[i];
	}
	return ret;
}

void mtrx::AbstractMatrix::SetSize(unsigned size)
{
	data.resize(size);
	for (vector<int> line : data) {
		line.resize(size);
	}
}

void AbstractMatrix::SetData(vector<vector<int>> newData) {
	data.clear();
	data = newData;
	this->size = newData.size();
}

void AbstractMatrix::PrintMatrix() {
	for (std::vector<int> line : data) {
		for (int value : line)
			std::cout << value << "\t";
		std::cout << std::endl;
	}
}

vector<int>& AbstractMatrix::operator [](unsigned i) { return data[i]; }
mtrx::AbstractMatrix::operator mtrx::Matrix()
{
	return Matrix(data);
}

AbstractMatrix AbstractMatrix::operator -() {
	AbstractMatrix result(data);
	for (vector<int>& line : result.data)
		for (int& value : line)
			value *= -1;
	return result;
}

bool mtrx::AbstractMatrix::operator==(AbstractMatrix A)
{
	if (size != A.getSize())
		throw IncompatibleMatricesException();

	for (unsigned i = 0; i < size; i++)
		for (unsigned j = 0; j < size; j++)
			if ((*this)[i][j] != A[i][j])
				return false;
	return true;
}

AbstractMatrix AbstractMatrix::operator +(AbstractMatrix A){
	
	if (size != A.getSize())
		throw IncompatibleMatricesException();

	AbstractMatrix result(size);
	
	for (unsigned i = 0; i < size; i++){
		for (unsigned j = 0; j < size; j++)
			result[i][j] = (*this)[i][j] + A[i][j];
	}
	return result;
}

AbstractMatrix AbstractMatrix::operator -(AbstractMatrix A) {

	if (size != A.getSize())
		throw IncompatibleMatricesException();

	AbstractMatrix result(size);

	for (unsigned i = 0; i < size; i++) {
		for (unsigned j = 0; j < size; j++)
			result[i][j] = (*this)[i][j] - A[i][j];
	}
	return result;
}

AbstractMatrix AbstractMatrix::operator *(AbstractMatrix A) {
	// verification of the size of the two matrices
	if (size != A.getSize())
		throw IncompatibleMatricesException();
	
	AbstractMatrix result(size);

	for (unsigned i = 0; i < size; i++)
		for (unsigned j = 0; j < size; j++)
			for (unsigned k = 0; k < size; k++)
				result[i][j] += (*this)[i][k] * A[k][j];
	return result;
}

AbstractMatrix AbstractMatrix::operator*(int coef)
{
	AbstractMatrix result(data);
	for (vector<int>& line : result.data)
		for (int& value : line)
			value *= coef;
	return result;
}

AbstractMatrix mtrx::AbstractMatrix::operator^(unsigned degree)
{
	AbstractMatrix ret = AbstractMatrix(this->size);
	for (unsigned i = 0; i < degree; i++)
		ret = ret*(*this);
	return ret;
}
