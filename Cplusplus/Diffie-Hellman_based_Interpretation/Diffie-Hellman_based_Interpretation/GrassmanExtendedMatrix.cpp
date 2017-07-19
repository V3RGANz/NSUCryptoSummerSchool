#include <fstream>
#include "Header.h"
#include "Matrix.h"
using namespace protocol;

GrassmanExtendedMatrix::GrassmanExtendedMatrix(vector<vector<int>> source) {
	this->source = source;
	this->SetSize(17);
	CombTraverse();

	for (size_t i = 0; i < 17; i++) {
		for (size_t j = 0; j < 17; j++) {
			(*this)[i][j] = (
				source[basis[i][main][0] - 1][basis[j][main][0] - 1] *
				source[basis[i][main][1] - 1][basis[j][main][1] - 1] *
				source[basis[i][main][2] - 1][basis[j][main][2] - 1] -
				source[basis[i][excess][0] - 1][basis[j][main][0] - 1] *
				source[basis[i][excess][1] - 1][basis[j][main][1] - 1] *
				source[basis[i][excess][2] - 1][basis[j][main][2] - 1]
				) % modulo;
			if ((*this)[i][j] < 0)
				(*this)[i][j] += modulo;
		}
	}
}

GrassmanExtendedMatrix::~GrassmanExtendedMatrix() {}

void GrassmanExtendedMatrix::printsource(ofstream& fout) {
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++)
			fout << right << source[i][j] << "\t";
		fout << endl << endl;
	}
}

void GrassmanExtendedMatrix::print(ofstream& fout) {
	fout << "basis:\n";

	for (size_t i = 0; i < 17; i++)
		fout << "e" << basis[i][main][0] << " e" << basis[i][main][1] << " e" << basis[i][main][2] << endl;

	for (size_t i = 0; i < 17; i++) {
		for (size_t j = 0; j < 17; j++)
			fout << right << (*this)[i][j] << "\t";
		fout << endl << endl;
	}
}

void GrassmanExtendedMatrix::CombTraverse() {

	if (level < N) {
		for (unsigned i = c.size(); i <= M; i++) {
			level++;
			CombTraverse();
			level--;
			c.push_back(level);
		}
		while (!c.empty() && c.back() == level)
			c.pop_back();
	}
	else {
		for (size_t i = c.size(); i < M; i++)
			c.push_back(N);

		PermutationTraverse();

		while (!c.empty() && c.back() == level)
			c.pop_back();
		if (permutations.size()) {
			for (vector<unsigned> perm : permutations) {
				basis.push_back({ perm, exss });
				index++;
			}
		}
		permutations.clear();
	}
}

void GrassmanExtendedMatrix::PermutationTraverse() {

	if (c[0] != c[1] && c[0] != c[2] && c[1] != c[2]) {
		permutations.push_back({ c[0], c[1], c[2] });
		permutations.push_back({ c[0], c[2], c[1] });
		permutations.push_back({ c[1], c[0], c[2] });
		permutations.push_back({ c[1], c[2], c[0] });
		permutations.push_back({ c[2], c[0], c[1] });
		exss = { c[2], c[1], c[0] };
	}
	else if (c[0] != c[1] || c[1] != c[2]) {
		if (c[0] == c[1]) {
			permutations.push_back({ c[0], c[0], c[2] });
			permutations.push_back({ c[2], c[0], c[0] });
			exss = { c[0], c[2], c[0] };
		}
		else if (c[0] == c[2]) {
			permutations.push_back({ c[0], c[0], c[1] });
			permutations.push_back({ c[1], c[0], c[0] });
			exss = { c[0], c[1], c[0] };
		}
		else {
			permutations.push_back({ c[0], c[1], c[1] });
			permutations.push_back({ c[1], c[1], c[0] });
			exss = { c[1], c[0], c[1] };
		}
	}
}
