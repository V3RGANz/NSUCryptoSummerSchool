#include <iostream>
#include "Header.h"
using namespace std;
using namespace mtrx;


int minvalue = 100;
int maxvalue = -100;

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

	protocol::human Alice;
	protocol::human Bob;
	protocol::pair keyExchange(Alice, Bob);

	keyExchange.protocol(5, 2);
	if (checkKeys(Alice, Bob))
		cout << "Bob and Alice have the same key!\n" << endl;
	else
		cout << "Bob and Alice have different keys ;(\n" << endl;
	return 0;
}