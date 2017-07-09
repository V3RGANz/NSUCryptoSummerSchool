#include <stdio.h>
#include "Header.h"
using namespace std;
using namespace mtrx;
int main(void) {
	
	Matrix A( {
		{5, -2, 0},
		{1,  3, 2},
		{4,  1, 7}
	});

	GrassmanMatrix B(A, 2);

	B.PrintMatrix();

	return 0;
}