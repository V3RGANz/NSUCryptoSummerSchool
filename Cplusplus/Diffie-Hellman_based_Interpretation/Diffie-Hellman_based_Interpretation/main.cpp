#include <iostream>
#include "Header.h"
using namespace std;
using namespace mtrx;
int main(void) {
	
	//Matrix A( {
	//	{5, -2, 0},
	//	{1,  3, 2},
	//	{4,  1, 7}
	//});

	//GrassmanMatrix B(A, 2);

	//B.PrintMatrix();

	Matrix X(5, -10, 10);

	cout << "source matrix: " << endl;
	X.PrintMatrix();
	cout << endl;

	GrassmanMatrix Y(X, 3);

	Y.PrintMatrix();

	human::human Alice;
	human::human Bob;

	return 0;
}