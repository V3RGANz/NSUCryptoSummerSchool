#ifndef PolyHeader
#define PolyHeader
#include <vector>
#include <ctime>
#include <stdlib.h>
using namespace std;

namespace poly {
	class Polynom
	{
	private:
		unsigned degree;
		vector<int> coefficients;

	public:
		//Creates undefined polynom
		Polynom();
		//Creates polynom with following coefficients with degree coefficients.size() -1
		Polynom(vector<int> coefficients);
		//Creates polynom with all coefficients = coef of following degree
		Polynom(int coef, unsigned degree);
		//Creates random polynom of following degree
		Polynom(unsigned degree);
		~Polynom();

		unsigned getDegree();

		int operator [](unsigned);


	};
}
#endif // PolyHeader