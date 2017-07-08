#include <stdio.h>
#include "Header.h"
using namespace std;
using namespace Grassman;
int main(void) {
	
	Matrix A( {
		{5, -2, 0},
		{1,  3, 2},
		{4,  1, 7}
	});

	GrassmanMatrix B(A, 2);

	B.PrintMatrix();
	A.PrintMatrix();
	printf("%d\n", A.getW());


	return 0;
}