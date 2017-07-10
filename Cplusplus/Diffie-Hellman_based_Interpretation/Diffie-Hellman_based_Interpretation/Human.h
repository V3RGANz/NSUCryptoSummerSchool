#pragma once
#include "Header.h"
namespace human {
	class human {

	private:
		/*
		common section
		*/
		mtrx::Matrix A;
		mtrx::Matrix B;
		poly::Polynom f[2];
		mtrx::Matrix publicMatrix;
		mtrx::Matrix key;
	};
}