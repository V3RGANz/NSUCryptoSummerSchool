#pragma once
//#ifndef PolyHeader
//#define PolyHeader
#include <vector>
#include <ctime>
#include <stdlib.h>

namespace protocol {
	class Polynom
	{
	private:
		unsigned degree;
		std::vector<int> coefficients;
	public:
		//Creates undefined polynom
		Polynom();
		//Creates random polynom of following degree
		Polynom(unsigned degree);
		~Polynom();

		//Compute result of current polynom of following matrix
		Matrix operator()(Matrix A);

		unsigned getDegree();
	};
}
//#endif // PolyHeader