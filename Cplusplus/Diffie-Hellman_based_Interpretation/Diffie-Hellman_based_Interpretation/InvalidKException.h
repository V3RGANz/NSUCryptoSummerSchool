#pragma once

#include<exception>

class InvalidKException : public std::exception
{

public:
	InvalidKException() throw() { }
	~InvalidKException() throw() { }

	const char* what() const throw() {
		return "K should be not bigger than matrix size and not equal to 0";
	}
private:

};
