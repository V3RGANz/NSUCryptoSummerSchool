#include <iostream>
#include <fstream>
#include "incompatibleMatricesException.h"
#include "Header.h"
#include "Matrix.h"

using namespace protocol;

AbstractMatrix::AbstractMatrix() { SetData({}); }

AbstractMatrix::AbstractMatrix(unsigned size)
{
	vector<vector<int> > newData(size, vector<int>(size, 0));
	unsigned j = 0;
	for (unsigned i = 0; i < size; i++) {
		newData[i][j++] = 1;
	}
	SetData(newData);
}

AbstractMatrix::~AbstractMatrix() {}

unsigned AbstractMatrix::getSize() { return size; }
vector<vector<int>> AbstractMatrix::getData() { return data;}

void AbstractMatrix::SetSize(unsigned size)
{
	data.resize(size);
	for (vector<int>& line : data) {
		line.resize(size);
	}
	this->size = size;
}

void AbstractMatrix::SetData(vector<vector<int>> newData) {
	data.clear();
	data = newData;
	this->size = newData.size();
}

void AbstractMatrix::Print(ofstream& out) {
	for (std::vector<int> line : data) {
		for (int value : line)
			out << value << "\t";
		out << std::endl << std::endl;
	}
}

vector<int>& AbstractMatrix::operator [](unsigned i) { return data[i]; }
AbstractMatrix::operator Matrix() { return Matrix(data); }

AbstractMatrix AbstractMatrix::operator -() {
	vector<vector<int> > data;
	for (vector<int>& line : data)
		for (int& value : line)
			value *= -1;
	AbstractMatrix result;
	result.SetData(data);
	return result;
}


bool AbstractMatrix::operator==(AbstractMatrix A)
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

	vector<vector<int> > newdata(size, vector<int>(size));
	
	for (unsigned i = 0; i < size; i++){
		for (unsigned j = 0; j < size; j++)
			newdata[i][j] = (modulo + ((*this)[i][j] + A[i][j]) % modulo) % modulo;
	}
	AbstractMatrix result;
	result.SetData(newdata);
	return result;
}

AbstractMatrix AbstractMatrix::operator -(AbstractMatrix A) {

	if (size != A.getSize())
		throw IncompatibleMatricesException();

	vector<vector<int> > newdata(size, vector<int>(size));

	for (unsigned i = 0; i < size; i++) {
		for (unsigned j = 0; j < size; j++)
			newdata[i][j] = (modulo + ((*this)[i][j] - A[i][j])%modulo)%modulo;
	}
	AbstractMatrix result;
	result.SetData(newdata);
	return result;
}

AbstractMatrix AbstractMatrix::operator *(AbstractMatrix A) {
	// verification of the size of the two matrices
	if (size != A.getSize())
		throw IncompatibleMatricesException();
	
	vector<vector<int> > newdata(size, vector<int>(size, 0));

	for (unsigned i = 0; i < size; i++)
		for (unsigned j = 0; j < size; j++)
			for (unsigned k = 0; k < size; k++)
				newdata[i][j] = (modulo + (newdata[i][j] + (*this)[i][k] * A[k][j]) % modulo) % modulo;
	AbstractMatrix result;
	result.SetData(newdata);
	return result;
}

AbstractMatrix AbstractMatrix::operator*(int coef)
{
	vector<vector<int> > newdata = data;
	for (vector<int>& line : newdata)
		for (int& value : line)
			value = (modulo + coef*value% modulo) % modulo;
	AbstractMatrix result;
	result.SetData(newdata);
	return result;
}

AbstractMatrix AbstractMatrix::operator^(unsigned degree)
{
	AbstractMatrix ret = AbstractMatrix(this->size);
	for (unsigned i = 0; i < degree; i++)
		ret = ret*(*this);
	return ret;
}

AbstractMatrix AbstractMatrix::operator%(int mod) {
	vector<vector<int> > newdata = data;
	for (vector<int>& line : newdata)
		for (int& value : line)
			value %= mod;
	AbstractMatrix result;
	result.SetData(newdata);
	return result;
}
