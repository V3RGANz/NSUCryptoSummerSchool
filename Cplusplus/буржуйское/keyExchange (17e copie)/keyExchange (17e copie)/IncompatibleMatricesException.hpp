#ifndef INCOMPATIBLEMATRICESEXCEPTION_HPP
#define INCOMPATIBLEMATRICESEXCEPTION_HPP

#include <exception>

using namespace std;

class IncompatibleMatricesException : public exception 
{
	
	public :
		IncompatibleMatricesException() throw() { }
		~IncompatibleMatricesException() throw() { }
	
		const char* what() const throw() {
			return "IncompatibleMatricesException";
		}
		
	private :
	
};

#endif
