#pragma once

namespace protocol {
	bool checkKeys(human h1, human h2);
	class human {
	public:
		human();
		~human();
		// public method that returns public key based on following parametrs
		Matrix makePublicKey(Matrix A, Matrix B, Matrix W, unsigned k);
		// make private key based on partner's public key
		void makePrivateKey(Matrix conjugate);
	private:
		// compare identity of private keys 
		friend bool checkKeys(human h1, human h2);
		Polynom f;
		Polynom g;
		Polynom h;
		Polynom u;
		Matrix left;
		Matrix right;
		Matrix key;
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
		Matrix A;
		Matrix B;
		Matrix W;
	}; // public section of protocol

}
