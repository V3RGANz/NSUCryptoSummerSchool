#include "Header.h"
#include "Matrix.h"
#include "Polynom.h"
#include "Human.h"

using namespace protocol;

human::human(){}
human::~human(){}

Matrix human::makePublicKey(Matrix A, Matrix B, Matrix W, unsigned k)
{
	f = Polynom(A.getSize());
	h = Polynom(A.getSize());
	GrassmanMatrix grA(f(A), k);
	GrassmanMatrix grB(h(B), k);
	g = Polynom(grA.getSize());
	u = Polynom(grB.getSize());
	left = g(grA);
	right = u(grB);
	return left*W*right;
}

void human::makePrivateKey(Matrix conjugate)
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
	W = Matrix(C(size, k), minvalue, maxvalue);
	Matrix Alice_open(Alice.makePublicKey(A, B, W, k));
	Matrix Bob_open(Bob.makePublicKey(A, B, W, k));
	Bob.makePrivateKey(Alice_open);
	Alice.makePrivateKey(Bob_open);
}

bool protocol::checkKeys(human h1, human h2) { return h1.key == h2.key; }
