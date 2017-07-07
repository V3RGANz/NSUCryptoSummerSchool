#ifndef OUTOFBOUNDSEXCEPTION_HPP
#define OUTOFBOUNDSEXCEPTION_HPP

#include <exception>

using namespace std;

class OutOfBoundsException : public exception 
{
	
	public :
		OutOfBoundsException() throw() { }
		~OutOfBoundsException() throw() { }
	
		const char* what() const throw() {
			return "OutOfBoundsException";
		}
		
	private :
	
};

#endif
