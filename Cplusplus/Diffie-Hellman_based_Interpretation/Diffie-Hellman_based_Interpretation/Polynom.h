#pragma once
#include "Matrix.h"

namespace poly {
	class Polynom
	{
	public:
		//Creates undefined polynom
		Polynom();
		//Creates polynom with following coefficients with degree coefficients.size() -1
		Polynom(vector<int> coefficients) {
			this->coefficients.clear();
			this->coefficients = coefficients;
			degree = coefficients.size();
		}
		//Creates polynom with all coefficients = coef of following degree
		Polynom(int coef, unsigned degree) {
			this->degree = degree;
			coefficients = vector<int>(degree, coef);
		}
		//Creates random polynom of following degree
		Polynom(unsigned degree) {
			//rand
		}
		~Polynom();

	private:
		unsigned degree;
		vector<int> coefficients;
	};
}
poly::Polynom::Polynom()
{
}

poly::Polynom::~Polynom()
{
}