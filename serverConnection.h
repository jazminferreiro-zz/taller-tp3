#ifndef __SERVER_H__
#define __SERVER_H__

#include "serverIndex.h"
#include "commonError.h"
#include "commonProtocol.h"
#include <string>


using std::cout;
using std::endl;
using std::string;

class Connection: Protocol{
	Index & index;
	void push();
	void tag();
	void pull();
public:
	Connection(Socket peer, Index & index);	
	void run();
	void stop();
};

#endif
