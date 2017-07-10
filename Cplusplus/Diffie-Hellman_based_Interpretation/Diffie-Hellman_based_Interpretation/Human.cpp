#include "Human.h"
using namespace protocol;
using namespace mtrx;
using namespace poly;
human::human(){}

human::human(unsigned degree) {}

protocol::human::~human(){}

Matrix human::makeOpenKey(Matrix A, Matrix B, Matrix W, unsigned k)
{
	f[0] = Polynom(A.getSize() - 1);
	f[2] = Polynom(A.getSize() - 1);
	GrassmanMatrix grA(A.computePoly(f[0]), k);
	GrassmanMatrix grB(B.computePoly(f[0]), k);
	f[1] = Polynom(grA.getSize() - 1);
	f[3] = Polynom(grB.getSize() - 1);
	left = grA.computePoly(f[2]);
	right = grB.computePoly(f[3]);
	return left*W*right;
}

void protocol::human::makePrivateKey(mtrx::Matrix conjugate)
{
	key = left*conjugate*right;
}


protocol::pair::pair(human & first, human & second) : Alice(first), Bob(second)
{
	Alice = first;
	Bob = second;
}

protocol::pair::~pair() { }

void protocol::pair::protocol(unsigned size, unsigned k)
{
	this->k = k;
	A = Matrix(size, minvalue, maxvvalue);
	B = Matrix(size, minvalue, maxvvalue);
	W = Matrix(
		[size](unsigned k) -> unsigned {
		if (size < k)
			return 0;
		if (k > size >> 1)
			k = size - k;
		unsigned ret = 1;
		for (unsigned i = 1; i <= k; ++i)
			ret = ret * (size - k + i) / i;
		return ret;
	}(k)
		, minvalue, maxvvalue);
	Bob.makePrivateKey(Alice.makeOpenKey(A, B, W, k));
	Alice.makePrivateKey(Bob.makeOpenKey(A, B, W, k));
}

bool protocol::checkKeys(human h1, human h2) { return h1.key == h2.key; }
