#include <iostream>
#include <fstream>
#include <string>
#include "Header.h"

#include "Matrix.h"
#include "Polynom.h"
#include "User.h"

using namespace std;

int modulo = 101;

// ìîäóëüíóþ àðèôìåòèêó ÿ áû êîíå÷íî åùå ïîôèêñèë

int main(void) {
	cout << "wtf";
/*
	if (argc != 2) {
		cout << "Incorrect args" << endl;
		exit(1);
	}

	if (argv[1][0] == 's') {
		unsigned size = 5;
		unsigned k = 3;

		Matrix A(size, modulo);
		Matrix B(size, modulo);
		unsigned size2 = C(size, k);
		Matrix W(size2, modulo);
		ofstream src("Source.bin", ofstream::binary);

		A.WriteToStream(&src);
		B.WriteToStream(&src);
		W.WriteToStream(&src);
		char numbers[] = { k, 2 };
		src.write(numbers, 2);
		src.close();
		return 0;
	}
	else if (argv[1][0] == 'o') {
		ifstream src("Source.bin", ifstream::binary);
		Matrix A(&src);
		Matrix B(&src);
		Matrix W(&src);
		char k;
		src.read(&k, 1);

		Polynom f(A.getSize());
		Polynom h(B.getSize());
		GrassmanMatrix grA(f(A), k);
		GrassmanMatrix grB(h(B), k);
		Polynom g(grA.getSize());
		Polynom u(grB.getSize());
		Matrix left = g(grA);
		Matrix right = u(grB);
		Matrix result = left*W*right;
		src.close();
		ofstream prv("PrivateData.bin", ifstream::binary);
		left.WriteToStream(&prv);
		right.WriteToStream(&prv);
		prv.close();
		ofstream pb("Public.bin", ofstream::binary);
		result.WriteToStream(&pb);
		pb.close();
		return 0;
	}
	else if (argv[1][0] == 'c') {
		ifstream prv("PrivateData.bin", ifstream::binary);
		Matrix left(&prv);
		Matrix right(&prv);
		prv.close();

		ifstream pbl("Public.bin", ifstream::binary);
		Matrix PubKey(&pbl);
		pbl.close();

		Matrix PrivateKey = left*PubKey*right;
		ofstream prk("PrivateKey.bin", ofstream::binary);
		PrivateKey.WriteToStream(&prk);
		prk.close();
		return 0;
	}
*/


	ofstream test("test.bin", ofstream::binary);
	Matrix({
		{2, 3, 4},
		{4, 9, 10},
		{23, 2311, 390}
	}).WriteToStream(&test);
	test.close();

	//unsigned k = 4;
	//char c;
	//do {
	//	cout << "wtf2";
	//	cin >> c;
	//	switch (c) {
	//	case 's': 
	//		cout << "wtf3";
	//	{
	//			unsigned size = 5;
	//			unsigned k = 3;

	//			Matrix A(size, modulo);
	//			Matrix B(size, modulo);
	//			unsigned size2 = C(size, k);
	//			Matrix W(size2, modulo);
	//			ofstream src("Source.bin", ofstream::binary);

	//			A.WriteToStream(&src);
	//			B.WriteToStream(&src);
	//			W.WriteToStream(&src);
	//			char numbers[] = { k, 2 };
	//			src.write(numbers, 2);
	//			src.close();
	//			return 0;
	//		}
	//	break;
	//	case 'o':
	//		cout << "wtf4";
	//		{
	//			ifstream src("Source.bin", ifstream::binary);
	//			Matrix A(&src);
	//			Matrix B(&src);
	//			Matrix W(&src);
	//			char k;
	//			src.read(&k, 1);

	//			Polynom f(A.getSize());
	//			Polynom h(B.getSize());
	//			GrassmanMatrix grA(f(A), k);
	//			GrassmanMatrix grB(h(B), k);
	//			Polynom g(grA.getSize());
	//			Polynom u(grB.getSize());
	//			Matrix left = g(grA);
	//			Matrix right = u(grB);
	//			Matrix result = left*W*right;
	//			src.close();
	//			ofstream prv("PrivateData.bin", ofstream::binary);
	//			left.WriteToStream(&prv);
	//			right.WriteToStream(&prv);
	//			prv.close();
	//			ofstream pb("Public.bin", ofstream::binary);
	//			result.WriteToStream(&pb);
	//			pb.close();
	//		}
	//		break;
	//	case 'c':
	//		cout << "wtf5";
	//		{
	//			ifstream prv("PrivateData.bin", ifstream::binary);
	//			Matrix left(&prv);
	//			Matrix right(&prv);
	//			prv.close();

	//			ifstream pbl("Public.bin", ifstream::binary);
	//			Matrix PubKey(&pbl);
	//			pbl.close();

	//			Matrix PrivateKey = left*PubKey*right;
	//			ofstream prk("PrivateKey.bin", ofstream::binary);
	//			PrivateKey.WriteToStream(&prk);
	//			prk.close();
	//		}
	//		break;
	//	}
	//} while (c != 'e');

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
	/*for (size_t i = 0; i < 10; i++)
	{
	 	clock_t begin  = clock();
	 	cout << "Protocol checking...\n";

	 	protocol::user Alice;
	 	protocol::user Bob;
	 	protocol::pair keyExchange(Alice, Bob);
	 	try {
	 		keyExchange.protocol(5, 3);
	 	}
	 	catch (const exception& ex) {
	 		cout << ex.what() << endl;
	 	}
	 	clock_t end = clock();
		
	 	if (checkKeys(Alice, Bob))
	 		cout << "Bob and Alice have the same key!" << endl;
	 	else
	 		cout << "Bob and Alice have different keys ;(" << endl;
	 	cout << "computing took " << (end - begin)/CLOCKS_PER_SEC << " seconds" << endl;
	 }*/

	//Grassman extended check
	//GrassmanExtendedMatrix A({
	//	{ 41, 85, 72 },
	//	{ 38, 80, 69 },
	//	{ 65, 68, 96 }
	//});

	//ofstream fout("source.txt");
	//A.printsource(fout);
	//fout.close();

	//ofstream f2out("new.txt");
	//A.print(f2out);
	//f2out.close();

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