#include "Header.h"
#include "Matrix.h"
using namespace protocol;

protocol::GrassmanExtendedMatrix::GrassmanExtendedMatrix()
{
}

protocol::GrassmanExtendedMatrix::GrassmanExtendedMatrix(Matrix a, unsigned k, unsigned m)
{
	K = k;
	M = m;
	A = a;
	repeatition = 0;
	Index[0].clear();
	Index[1].clear();
	line = 0;
	column = 0;
	LoopState = true;
	Loop(K);
}
GrassmanExtendedMatrix::~GrassmanExtendedMatrix() {}
