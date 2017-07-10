#include "Polynom.h"

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
		value = rand();
	}
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