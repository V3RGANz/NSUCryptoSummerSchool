#include "Matrix.h"
#include <iostream>
using namespace mtrx;

GrassmanMatrix::GrassmanMatrix(vector<vector<int> > Data) : GrassmanExtendedMatrix() {
	SetData(Data);
}

GrassmanMatrix::GrassmanMatrix(Matrix M, unsigned k) : GrassmanExtendedMatrix() {

	// source matrix shouldn't be less than k*k
	if (k > M.getSize()) {
		this->SetData({});
		return;
	}

	this->M = M;
	K = k;

	unsigned size = C(M.getSize(), k);
	vector<int> lines(size);
	vector<vector<int> > newData(size, lines);
	SetData(newData);
	LineIndex = *(new vector<unsigned>);
	column = 0;
	line = 0;
	Loop(K);
	LineIndex.clear();
	ColumnIndex.clear(); //hqxzrr2
	LoopState = true;

}
GrassmanMatrix::GrassmanMatrix() : GrassmanExtendedMatrix() {}

unsigned GrassmanMatrix::C(unsigned n, unsigned k) {
	if (n < k)
		return 0;
	if (k > n >> 1)
		k = n - k;
	unsigned ret = 1;
	for (unsigned i = 1; i <= k; ++i)
		ret = ret * (n - k + i) / i;
	return ret;
}

void GrassmanMatrix::Loop(unsigned n) {
	if (n != 0) {
		//defining current index list
		vector<unsigned>* Index = LoopState ? &LineIndex : &ColumnIndex;
		unsigned i = (*Index).empty() ? 0 : (*Index).back() + 1;

		// recursive combinations search
		(*Index).push_back(i);
		for (; i <= M.getSize() - n; i++) {
			Loop(n - 1);
			(*Index).back()++;
		}
		(*Index).pop_back();
	}
	else {
		if (LoopState) {
			LoopState = false;
			Loop(K);
			LoopState = true;
		}
		else
			(*this)[line][column] = Minor(K);
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
		return M[LineIndex.back()][ColumnIndex.back()];
	}
	else {
		int sign = 1;
		int determinant = 0;
		for (unsigned i = 0; i < ColumnIndex.size(); i++) {
			unsigned temporaryIndex = ColumnIndex[i];
			ColumnIndex.erase(ColumnIndex.begin() + i);
			determinant += sign * M[LineIndex[K - j]][temporaryIndex] * Minor(j - 1);
			ColumnIndex.insert(ColumnIndex.begin() + i, temporaryIndex);
			sign *= -1;
		}
		return determinant;
	}
}

GrassmanMatrix::~GrassmanMatrix() {}