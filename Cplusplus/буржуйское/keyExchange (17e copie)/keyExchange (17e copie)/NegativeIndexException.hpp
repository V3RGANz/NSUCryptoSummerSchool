#ifndef NEGATIVEINDEXEXCEPTION_HPP
#define NEGATIVEINDEXEXCEPTION_HPP

#include <exception>

using namespace std;

class NegativeIndexException : public exception 
{
	
	public :
		NegativeIndexException() throw() { }
		~NegativeIndexException() throw() { }
	
		const char* what() const throw() {
			return "NegativeIndexException";
		}
		
	private :
	
};

#endif
