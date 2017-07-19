#include <iostream>
#include <fstream>
#include "Header.h"

#include "Matrix.h"
#include "Polynom.h"
#include "User.h"

using namespace std;

int modulo = 101;

// модульную арифметику я бы конечно еще пофиксил

int main(void) {
	unsigned k = 3;
	//Grassman check 1
	/*{
		Matrix A({
			{5, -2, 0},
			{1,  3, 2},
			{4,  1, 7}
		});

		GrassmanMatrix B(A, 2);

		B.Print();
	}*/
	//Grassman check 2
	/*{
		Matrix X(5, -10, 10);
		cout << "source matrix: " << endl;
		X.Print();
		cout << "representation in Grassman Algebra (k = " << k << "): " << endl;
		GrassmanMatrix Y(X, k);
		Y.Print();
	}*/
	//Protocol check
	{
		cout << "Protocol checking...\n";

		protocol::user Alice;
		protocol::user Bob;
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
	}

	//Grassman extended check

	//GrassmanExtendedMatrix A({
	//	{ 41, 85, 72 },
	//	{ 38, 80, 69 },
	//	{ 65, 68, 96 }
	//});

	//GrassmanExtendedMatrix B({
	//	{ 59, 69, 4  },
	//	{ 29, 64, 8  },
	//	{ 10, 10, 50 }
	//});

	//ofstream fout("second_source.txt");
	//B.printsource(fout);
	//fout.close();

	//ofstream f2out("second.txt");
	//B.print(f2out);
	//f2out.close();

	//ofstream f3out("result.txt");
	//Matrix result = (A*B) % 101;
	//result.Print(f3out);
	//f3out.close();

	return 0;
}