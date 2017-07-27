#include "Header.h"
#include "Polynom.h"
#include "Matrix.h"
#include <fstream>

Matrix::Matrix() : AbstractMatrix() {}
Matrix::Matrix(unsigned size) : AbstractMatrix(size) { }
Matrix::Matrix(unsigned size, unsigned modulo) {
	// можно было бы сделать короче если было бы доступно поле data
	vector<vector<int> > newData(size, vector<int>(size));
	for (vector<int>& line : newData)
		for (int& value : line)
			value = (modulo + rand() % modulo) % modulo;
	SetData(newData);
}
Matrix::Matrix(Polynom p, Matrix A) { *this = p(A); }

protocol::Matrix::Matrix(ifstream * input) {
	char size;
	input->read(&size, 1);
	SetSize(size);
	int* buffer = new int[size*size];
	input->read((char*)buffer, 4*size*size);
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			(*this)[i][j] = buffer[i*size + j];
	delete[] buffer;
}

void protocol::Matrix::WriteToStream(ofstream * output) {
	char size = getSize();
	int* buffer = new int[size*size];
	output->write(&size, 1);
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			buffer[i*size + j] = (*this)[i][j];
	output->write((char*)buffer, 4*size*size);
	delete[] buffer;
}

Matrix::Matrix(vector<vector<int> > Data) : AbstractMatrix() { SetData(Data); }
Matrix::~Matrix() {}

