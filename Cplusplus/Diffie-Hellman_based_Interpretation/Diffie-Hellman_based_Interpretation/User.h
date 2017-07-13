#pragma once

namespace protocol {
	bool checkKeys(user h1, user h2);
	class user {
	public:
		user();
		~user();
		// public method that returns public key based on following parametrs
		Matrix makePublicKey(Matrix A, Matrix B, Matrix W, unsigned k);
		// make private key based on partner's public key
		void makePrivateKey(Matrix conjugate);
	private:
		// compare identity of private keys 
		friend bool checkKeys(user h1, user h2);
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
		pair(user& first, user& second);
		~pair();
		// exchange public data between two humans for computing private key
		void protocol(unsigned size, unsigned k);
	private:
		user& Alice;
		user& Bob;
		unsigned size;
		unsigned K;
		Matrix A;
		Matrix B;
		Matrix W;
	}; // public section of protocol

}
