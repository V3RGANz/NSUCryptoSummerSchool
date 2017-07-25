#include <fstream>
#include "stdio.h"
#include "Header.h"
#include "Matrix.h"
#include "Polynom.h"
#include "User.h"

using namespace std;

extern "C" __declspec(dllexport) void _cdecl Server_GenerateSource() {
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
	char numbers[] = {k, 2};
	src.write(numbers, 2);
	src.close();
}
extern "C" __declspec(dllexport) void _cdecl MakePublic() {
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
	ofstream prv("PrivateData.bin", ofstream::binary);
	left.WriteToStream(&prv);
	right.WriteToStream(&prv);
	prv.close();
	ofstream pb("Public.bin", ofstream::binary);
	result.WriteToStream(&pb);
	pb.close();
}

extern "C" __declspec(dllexport) void _cdecl MakePrivate() {
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
}
