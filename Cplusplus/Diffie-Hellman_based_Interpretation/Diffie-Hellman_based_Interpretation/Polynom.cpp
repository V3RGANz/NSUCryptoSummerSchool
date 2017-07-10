#include "Polynom.h"

poly::Polynom::Polynom()
{
}

poly::Polynom::~Polynom()
{
}

unsigned poly::Polynom::getDegree()
{
	return degree;
}

int poly::Polynom::operator[](unsigned index)
{
	return coefficients[index];
}
