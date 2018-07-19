#ifndef __CLIENT_H__
#define __CLIENT_H__
#include "commonProtocol.h"
#include "error.h"
#include <stack> 
#include <string>
#include <stdio.h>
#include <iostream>


using std::cout;
using std::endl;
using std::string;

#define SERVER_NAME 1 
#define PORT 2
#define COMAND 3
#define FILE_NAME 4
#define HASH 5
#define TAG_NAME 4
class Client: Protocol{
	void push(const string & file_name, const string & hash);

	void tag(const string & tag_name, std::stack<string> & hashes);

	void pull(const string & tag_name);

public:
	Client(char * host_name, char * port);

	//ejecuta push tag o pull segun corresponda
	void execute_command(int argc, char * argv[]);
};

#endif
