#include "Polynom.h"
#include "Header.h"
poly::Polynom::Polynom(){}
poly::Polynom::Polynom(vector<int> coefficients) {
	this->coefficients.clear();
	this->coefficients = coefficients;
	degree = coefficients.size();
}
poly::Polynom::Polynom(int coef, unsigned degree) {
	this->degree = degree;
	coefficients = vector<int>(degree, coef);
}
poly::Polynom::Polynom(unsigned degree) {
	coefficients = vector<int>(degree);
	for (int& value : coefficients){
		value = minvalue + rand() % (maxvalue - minvalue + 1);
	}
	this->degree = degree;
}
poly::Polynom::~Polynom(){}

mtrx::Matrix poly::Polynom::operator()(mtrx::Matrix A)
{
	mtrx::Matrix ret(vector<vector<int> >(A.getSize(), vector<int>(A.getSize(), 0)));
	for (unsigned int i = 0; i < degree; i++)
		ret = ret + (A ^ i)*coefficients[i];
	return ret;
}

unsigned poly::Polynom::getDegree() { return degree; }