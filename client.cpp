#include "client.h"
#include <string> 
#include <stack> 

void Client::push(const string & file_name, const string & hash){
	send_code(PUSH_CODE);
	send_string(file_name);
	send_string(hash);
	if(receive_code() == OK){
		try{
			send_file(file_name);
		} catch(Error e){
			cout << "Error: archivo inexistente.\n";
		}
	}
}

void Client::tag(const string & tag_name, std::stack<string> & hashes){	
	send_code(TAG_CODE);
	send_size_first(hashes.size());
	send_string(tag_name);
	while (!hashes.empty()){
    	string hash = hashes.top();
     	hashes.pop();
     	send_string(hash);
  	}
  	if(receive_code()!= OK){
  		cout << "Error: tag/hash incorrecto.\n";
  	}
}

void Client::pull(const string & tag_name){
	send_code(PULL_CODE);
	send_string(tag_name);
	if(receive_code() == NOT_OK){ 
		cout << "Error: tag/hash incorrecto.\n";
		return;
	}
	int size_of_files = receive_size_first();
	for(int i = 0; i  < size_of_files; i++){
		string file_name = receive_string();
		receive_file(file_name);
	}
}

Client::Client(char * host_name, char * port)://
Protocol(std::move(Socket(host_name, port))){	
	this->socket.connection();
}

void Client::execute_command(int argc, char * argv[]){
	const string command = argv[COMAND];

	if (command.compare("pull") == 0){
		const string tag_name = argv[TAG_NAME];
		pull(tag_name);
		return;
	}
	if(argc <6){
		cout << "Error: argumentos invalidos.\n";
		return;
	}

	if (command.compare("push") == 0){
		const string file = argv[FILE_NAME];
		const string hash = argv[HASH];
		push(file, hash);
		return;
	}
	if (command.compare("tag") == 0){
		const string tag_name = argv[TAG_NAME];
		std::stack<string>  hashes;
		for(int i = TAG_NAME+1; i< argc; i++){
			hashes.push(argv[i]);
		}
		tag(tag_name,hashes);
		return;
	}
	cout << "Error: argumentos invalidos.\n";
}


