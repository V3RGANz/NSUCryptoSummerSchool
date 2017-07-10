#pragma once
#include "Header.h"
namespace human {

	class human {
	public:
		human(unsigned degree);
	private:
		unsigned degree;
		poly::Polynom f[2];
		mtrx::Matrix key;
	};

	class pair () {
	public:
		pair(human& first, human& second);
		protocol();
	private:
		human& pair[2];
		/*
		commom (public section);
		*/
		mtrx::Matrix A;
		mtrx::Matrix B;
	}
}