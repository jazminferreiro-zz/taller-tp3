#ifndef __ERROR_H__
#define __ERROR_H__


#include <iostream>
#include <cstdio>
#include <cstdarg>

#define BUF_LEN 256

class Error : public std::exception {
private:
	char msg[BUF_LEN];
public:
	explicit Error(const char * format,...) noexcept;
	virtual const char * what() const noexcept; 
	virtual ~Error() noexcept;	
};

#endif	
