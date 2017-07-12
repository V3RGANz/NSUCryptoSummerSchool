#include "Header.h"
#include "Matrix.h"
#include "Polynom.h"
#include "User.h"

using namespace protocol;

user::user(){}
user::~user(){}

Matrix user::makePublicKey(Matrix A, Matrix B, Matrix W, unsigned k)
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

void user::makePrivateKey(Matrix conjugate)
{
	key = left*conjugate*right;
}

protocol::pair::pair(user & first, user & second) : Alice(first), Bob(second)
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

bool protocol::checkKeys(user h1, user h2) { return h1.key == h2.key; }
