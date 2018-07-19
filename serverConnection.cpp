#include "serverConnection.h"
#include <set>
#include <string>


Connection::Connection(Socket peer, Index & index)://
Protocol(std::move(peer)), index(index){}
	

void Connection::run(){
	int code = receive_code();
	switch(code){
		case 1:
			push();
			break;
		case 2:
			tag();
			break;
		case 3:
			pull();
			break;
		throw Error("codigo %i no valido", code);
	}
}

void Connection::push(){
	string file_name = receive_string();
	string hash = receive_string();
	if((this->index).does_hash_exist(hash)){
		send_code(NOT_OK);
	}else {
		send_code(OK);
		if(receive_file(hash)){
			this->index.add_file_hash(file_name, hash);
		}
	}
}

void Connection::tag(){
	ssize_t hashes_size = receive_size_first();
	string tag_name = receive_string();
	bool already_exist = (this->index).get_tag_files(tag_name).size() > 0;
	bool invalid_hash = false;
	for (int i = 0; i < hashes_size; i++){
		string hash = receive_string();
		if (!(this->index).does_hash_exist(hash)){
			invalid_hash = true;
		} else {
			(this->index).add_tag_hash(tag_name, hash);
		}
	}
	if (already_exist || hashes_size <= 0 || invalid_hash){
		send_code(NOT_OK);
		return;
	}
	send_code(OK);
}

void Connection::pull(){
	string tag_name = receive_string();
	std::set<std::string,cmp> tag_files = (this->index).get_tag_files(tag_name);
	int size_of_files = tag_files.size();
	if(size_of_files == 0){
		send_code(NOT_OK);	
	}else {
		send_code(OK);
		send_size_first(size_of_files);
		
		for (std::set<std::string>::iterator//
            set_iter=tag_files.begin();//
            set_iter!=tag_files.end();//
            ++set_iter){
            string hash = (*set_iter);  
        	string name = this->index.get_file_name(hash);
        	send_string(name+"."+tag_name);
        	send_file(hash);
        }
	}	
}

void Connection::stop(){
	(this->socket).stop();
}



