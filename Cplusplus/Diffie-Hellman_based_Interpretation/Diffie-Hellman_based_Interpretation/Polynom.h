#ifndef PolyHeader
#define PolyHeader
#include <vector>

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

		unsigned getDegree();

		int operator [](unsigned);


	};
}
#endif // PolyHeader
