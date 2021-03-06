#include "Header.h"
#include "Matrix.h"
#include "InvalidKException.h"
#include <iostream>

GrassmanMatrix::GrassmanMatrix(Matrix M, unsigned k) : AbstractMatrix() {

	// source matrix shouldn't be less than k*k
	if (k == 0  || k > M.getSize()) {
		this->SetData({});
		throw InvalidKException();
	}

	this->M = M;
	K = k;

	unsigned size = C(M.getSize(), k);
	vector<int> lines(size);
	vector<vector<int> > newData(size, lines);
	SetData(newData);
	Index[0] = *(new vector<unsigned>);
	Index[1] = *(new vector<unsigned>);
	column = 0;
	line = 0;
	LoopState = 1;
	Loop(K);
	Index[0].clear();
	Index[1].clear(); //hqxzrr2

}

void GrassmanMatrix::Loop(unsigned n) {
	if (n != 0) {
		//defining current index list
		unsigned i = Index[LoopState].empty() ? 0 : Index[LoopState].back() + 1;
		// recursive combinations search
		Index[LoopState].push_back(i);
		for (; i <= M.getSize() - n; i++) {
			Loop(n - 1);
			Index[LoopState].back()++;
		}
		Index[LoopState].pop_back();
	}
	else {
		if (LoopState) {
			LoopState = 0;
			Loop(K);
			LoopState = 1;
		}
		else
			(*this)[line][column] = (modulo + Minor(K) % modulo) % modulo;
		if (LoopState) {
			line++;
			column = 0;
		}
		else
			column++;
	}
}
int GrassmanMatrix::Minor(unsigned j) {
	if (j == 1) {
		return M[Index[1].back()][Index[0].back()];
	}
	else {
		int sign = 1;
		int determinant = 0;
		for (unsigned i = 0; i < Index[0].size(); i++) {
			unsigned temporaryIndex = Index[0][i];
			Index[0].erase(Index[0].begin() + i);
			determinant += sign * M[Index[1][K - j]][temporaryIndex] * Minor(j - 1);
			Index[0].insert(Index[0].begin() + i, temporaryIndex);
			sign *= -1;
		}
		return determinant;
	}
}

GrassmanMatrix::~GrassmanMatrix() {}