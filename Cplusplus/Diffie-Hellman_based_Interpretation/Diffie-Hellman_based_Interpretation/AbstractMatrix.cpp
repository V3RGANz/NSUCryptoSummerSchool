#include "Matrix.h"
#include <iostream>

using namespace Grassman;


AbstractMatrix::AbstractMatrix() { SetData({}); }
AbstractMatrix::AbstractMatrix(unsigned height, unsigned width) { resize(height, width); }
AbstractMatrix::~AbstractMatrix() { data.clear(); }

unsigned AbstractMatrix::getH() {
	return h;
}
unsigned AbstractMatrix::getW() {
	return w;
}

void AbstractMatrix::setW(unsigned newW) {
	w = newW;
	data.resize(w);
}
void AbstractMatrix::setH(unsigned newH) {
	h = newH;
	for (vector<int>& line : data) {
		line.resize(h);
	}
}
void AbstractMatrix::resize(unsigned newH, unsigned newW) {
	setW(newW);
	setH(newH);
}
void AbstractMatrix::SetData(vector<vector<int>> newData) {
	data.clear();
	unsigned size = newData.size();
	setW(size);
	if (size) {
		data = newData;
		setH(data[0].size()); // expected that all columns aligned
	}
	else
		setH(0);
}

vector<int>& AbstractMatrix::operator [](unsigned i) { return data[i]; }


void AbstractMatrix::PrintMatrix() {
	for (std::vector<int> line : data){
		for (int value : line)
			std::cout << value << "\t";
		std::cout << std::endl;
	}
}
