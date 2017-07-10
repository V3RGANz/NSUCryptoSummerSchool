#include "Polynom.h"

poly::Polynom::Polynom(){}
Polynom(vector<int> coefficients) {
	this->coefficients.clear();
	this->coefficients = coefficients;
	degree = coefficients.size();
}
Polynom(int coef, unsigned degree) {
	this->degree = degree;
	coefficients = vector<int>(degree, coef);
}
poly::Polynom::~Polynom(){}

unsigned poly::Polynom::getDegree()
{
	return degree;
}

int poly::Polynom::operator[](unsigned index)
{
	return coefficients[index];
}