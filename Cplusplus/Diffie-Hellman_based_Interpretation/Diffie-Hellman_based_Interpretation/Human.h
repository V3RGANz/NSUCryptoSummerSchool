#pragma once
#include "Header.h"


namespace protocol {
	class human {
	public:
		human();
		human(unsigned degree);
		~human();
		mtrx::Matrix makeOpenKey(mtrx::Matrix A, mtrx::Matrix B, mtrx::Matrix W, unsigned k);
		void makePrivateKey(mtrx::Matrix conjugate);
	private:
		friend bool checkKeys(human h1, human h2);
		poly::Polynom f[4];
		mtrx::Matrix left;
		mtrx::Matrix right;
		mtrx::Matrix key;
	};

	class pair {
	public:
		pair(human& first, human& second);
		~pair();
		void protocol(unsigned size, unsigned k);
	private:
		human& Alice;
		human& Bob;
		/*
		commom properties(public section);
		*/
		unsigned size;
		unsigned k;
		mtrx::Matrix A;
		mtrx::Matrix B;
		mtrx::Matrix W;
	};

}
