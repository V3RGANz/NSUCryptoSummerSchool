#pragma once

#include<exception>

class UnsutableKException : public std::exception
{

public:
	UnsutableKException() throw() { }
	~UnsutableKException() throw() { }

	const char* what() const throw() {
		return "K should be not bigger than matrix size and not equal to 0";
	}
private:

};
