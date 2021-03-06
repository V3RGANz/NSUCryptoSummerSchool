#ifndef MAIN_HEADER
#define MAIN_HEADER

extern int modulo;

namespace protocol {
	class AbstractMatrix;
	class Matrix;	
	class GrassmanMatrix;
	class GrassmanExtendedMatrix;

	class Polynom;
	
	class user;
	
	class pair;
}

using namespace protocol;

//Returns the number of combinations of k elements from n-elements set
unsigned C(unsigned n, unsigned k);
#endif //MAIN_HEADER
