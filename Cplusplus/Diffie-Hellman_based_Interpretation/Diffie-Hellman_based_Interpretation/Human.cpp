#include "Human.h"
using namespace protocol;
using namespace mtrx;
using namespace poly;
human::human(){}

human::human(unsigned degree) {}

protocol::human::~human(){}

Matrix human::makeOpenKey(Matrix A, Matrix B, Matrix W, unsigned k)
{
	f = Polynom(A.getSize());
	h = Polynom(A.getSize());
	GrassmanMatrix grA(A.computePoly(f), k);
	GrassmanMatrix grB(B.computePoly(h), k);
	g = Polynom(grA.getSize());
	u = Polynom(grB.getSize());
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
	A = Matrix(size, minvalue, maxvalue);
	B = Matrix(size, minvalue, maxvalue);
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
		, minvalue, maxvalue);
	Matrix Alice_open(Alice.makeOpenKey(A, B, W, k));
	Matrix Bob_open(Bob.makeOpenKey(A, B, W, k));
	Bob.makePrivateKey(Alice_open);
	Alice.makePrivateKey(Bob_open);
}

bool protocol::checkKeys(human h1, human h2) { return h1.key == h2.key; }
