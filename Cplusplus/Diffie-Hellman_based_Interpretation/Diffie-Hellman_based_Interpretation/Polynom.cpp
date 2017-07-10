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
		value = rand() % (maxvalue - minvalue + 1);
	}
	this->degree = degree;
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