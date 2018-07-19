#include "commonError.h"

Error::Error(const char* format, ...) noexcept {
	va_list args;
	va_start(args, format);
	int s = vsnprintf(msg, BUF_LEN, format, args);
	msg[s+1] = 0;
	va_end(args);	
}

//devuelve el error
const char * Error::what() const noexcept{
	return msg;
}
Error::~Error() noexcept{}

