#pragma once

#include<exception>

class IncompatibleMatricesException : public std::exception
{

public:
	IncompatibleMatricesException() throw() { }
	~IncompatibleMatricesException() throw() { }

	const char* what() const throw() {
		return "IncompatibleMatricesException";
	}
private:

};
