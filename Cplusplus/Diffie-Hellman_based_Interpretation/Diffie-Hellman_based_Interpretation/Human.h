#pragma once
#include "Header.h"


namespace protocol {
	
	class human {
	public:
		human();
		// create human with polynoms of following degree
		human(unsigned degree);
		~human();
		// public method that returns public key based on following parametrs
		mtrx::Matrix makePublicKey(mtrx::Matrix A, mtrx::Matrix B, mtrx::Matrix W, unsigned k);
		// make private key based on partner's public key
		void makePrivateKey(mtrx::Matrix conjugate);
	private:
		friend bool checkKeys(human h1, human h2);
		poly::Polynom f;
		poly::Polynom g;
		poly::Polynom h;
		poly::Polynom u;
		mtrx::Matrix left;
		mtrx::Matrix right;
		mtrx::Matrix key;
	}; //private section of protocol

	class pair {
	public:
		pair(human& first, human& second);
		~pair();
		// exchange public data between two humans for computing private key
		void protocol(unsigned size, unsigned k);
	private:
		human& Alice;
		human& Bob;
		unsigned size;
		unsigned k;
		mtrx::Matrix A;
		mtrx::Matrix B;
		mtrx::Matrix W;
	}; // public section of protocol

}
