#include "Header.h"
#include "Polynom.h"
#include "Matrix.h"
Polynom::Polynom(){}

Polynom::Polynom(unsigned degree) {
	coefficients = vector<int>(degree);
	srand(time(NULL));
	for (int& value : coefficients){
		value = rand() % modulo;
	}
	this->degree = degree;
}
Polynom::~Polynom(){}

Matrix Polynom::operator()(Matrix A)
{
	Matrix ret(vector<vector<int> >(A.getSize(), vector<int>(A.getSize(), 0)));
	for (unsigned int i = 0; i < degree; i++)
		ret = ret + (A ^ i)*coefficients[i];
	return ret;
}

unsigned Polynom::getDegree() { return degree; }