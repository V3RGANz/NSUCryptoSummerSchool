#include <fstream>
#include "stdio.h"
#include "Header.h"
#include "Matrix.h"
#include "Polynom.h"
#include "User.h"

using namespace std;

extern "C" __declspec(dllexport) void _cdecl GeneratePublicData(unsigned size, unsigned k, unsigned mod) {
	/*unsigned size = 5;
	unsigned k = 3;*/
	ofstream log("log.txt", ofstream::app);
	log.clear();
	log << "taken modulo == \n" << mod;
	log.close();
	modulo = mod;

	Matrix A(size, modulo);
	Matrix B(size, modulo);
	unsigned size2 = C(size, k);
	Matrix W(size2, modulo);
	ofstream src("PublicData.bin", ofstream::binary);

	src.write((char*)&k, 1);
	src.write((char*)&modulo, 4);

	A.WriteToStream(&src);
	B.WriteToStream(&src);
	W.WriteToStream(&src);

	src.close();
}
extern "C" __declspec(dllexport) void _cdecl MakePublic() {
	ofstream log("log.txt", ofstream::app);
	log << "MakePublic \n";
	log.close();
	ifstream src("PublicData.bin", ifstream::binary);
	char k;
	src.read(&k, 1);
	src.read((char*)(&modulo), 4);
	log.open("log.txt", ofstream::app);
	log << "modulo = " << modulo << endl;
	log.close();
	Matrix A(&src);
	Matrix B(&src);
	Matrix W(&src);
	src.close();
	log.open("log.txt", ofstream::app);
	log << "Matrices read \n";
	log.close();
	Polynom f(A.getSize());
	Polynom h(B.getSize());
	log.open("log.txt", ofstream::app);
	log << "Polynoms created\n";
	log.close();
	GrassmanMatrix grA(f(A), k);
	GrassmanMatrix grB(h(B), k);
	log.open("log.txt", ofstream::app);
	log << "Grassman computed\n";
	log.close();
	Polynom g(grA.getSize());
	Polynom u(grB.getSize());
	log.open("log.txt", ofstream::app);
	log << "2d Polynoms created\n";
	log.close();
	Matrix left = g(grA);
	Matrix right = u(grB);
	log.open("log.txt", ofstream::app);
	log << "left and right computed\n";
	log.close();
	Matrix result = left*W*right;
	log.open("log.txt", ofstream::app);
	log << "result computed\n";
	log.close();
	ofstream prv("LeftRight.bin", ofstream::binary);
	left.WriteToStream(&prv);
	right.WriteToStream(&prv);
	prv.close();
	log.open("log.txt");
	log << "First file added\n";
	log.close();
	ofstream pb("YoursThenHisPiece.bin", ofstream::binary);
	result.WriteToStream(&pb);
	pb.close();
	log.open("log.txt");
	log << "Second file added\n";
	log.close();
}

extern "C" __declspec(dllexport) void _cdecl MakePrivate(unsigned mod) {
	modulo = mod;

	ofstream log("log.txt", ofstream::app);
	log << "taken modulo == " << mod << "\n";
	log.close();
	ifstream prv("LeftRight.bin", ifstream::binary);
	Matrix left(&prv);
	Matrix right(&prv);
	prv.close();

	ifstream pbl("YoursThenHisPiece.bin", ifstream::binary);
	Matrix PubKey(&pbl);
	pbl.close();

	Matrix PrivateKey = left*PubKey*right;
	ofstream prk("PrivateKey.bin", ofstream::binary);
	PrivateKey.WriteToStream(&prk);
	prk.close();
}
