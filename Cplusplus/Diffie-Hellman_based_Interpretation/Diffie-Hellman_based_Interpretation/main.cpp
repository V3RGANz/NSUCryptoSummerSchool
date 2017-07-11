#include <iostream>
#include "Header.h"
using namespace std;
using namespace mtrx;


int minvalue = 100;
int maxvalue = -100;

int main(void) {

	//Grassman check 1

	Matrix A( {
		{5, -2, 0},
		{1,  3, 2},
		{4,  1, 7}
	});

	GrassmanMatrix B(A, 2);

	B.PrintMatrix();

	//Grassman check 2

	Matrix X(5, -10, 10);
	unsigned k = 3;
	cout << "source matrix: " << endl;
	X.PrintMatrix();
	cout << "representation in Grassman Algebra (k = " << k << "): " << endl;
	GrassmanMatrix Y(X, k);
	Y.PrintMatrix();

	//Protocol check
	cout << endl << endl
		<< "Protocol checking...\n";

	protocol::human Alice;
	protocol::human Bob;
	protocol::pair keyExchange(Alice, Bob);
	try {
		keyExchange.protocol(5, k);
	}
	catch (const exception& ex) {
		cout << ex.what() << endl;
	}

	if (checkKeys(Alice, Bob))
		cout << "Bob and Alice have the same key!" << endl;
	else
		cout << "Bob and Alice have different keys ;(" << endl;

	return 0;
}