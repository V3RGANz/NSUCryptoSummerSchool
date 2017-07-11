#ifndef PolyHeader
#define PolyHeader
#include <vector>
#include <ctime>
#include <stdlib.h>
#include "Matrix.h"

namespace poly {
	class Polynom
	{
	private:
		unsigned degree;
		vector<int> coefficients;
	public:
		//Creates undefined polynom
		Polynom();
		//Creates random polynom of following degree
		Polynom(unsigned degree);
		~Polynom();

		//Compute result of current polynom of following matrix
		mtrx::Matrix operator()(mtrx::Matrix A);

		unsigned getDegree();
	};
}
#endif // PolyHeader