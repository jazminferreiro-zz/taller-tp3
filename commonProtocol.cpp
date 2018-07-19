#include "commonProtocol.h"
#include <string> 

Protocol::Protocol(Socket socket):socket(std::move(socket)) {}

void Protocol::send_code(int code_to_send){
	char code[PROTOCOL_CODE];
	snprintf(code,PROTOCOL_CODE, "%d", code_to_send);
  (this->socket).send_message(code,PROTOCOL_CODE);
}


void Protocol::send_string(const string & string_to_send){
	int size = string_to_send.size();
	send_size_first(size);
	(this->socket).send_message(string_to_send.c_str(),size);
}


void Protocol::send_file(const std::string & file_name){
    std::ifstream file(file_name,std::ifstream::binary);
    if (file.fail()){
      send_code(NOT_OK);
      throw Error("No se encontro el archivo %s\n", file_name.c_str());
    }
    send_code(OK);
    file.seekg(0, file.end);
    int file_len = file.tellg();
    file.seekg(0, file.beg);
    send_size_first(file_len);
    	
    int bytes_sent = 0;
  	int total_sent = 0;
  	char request[CHUNK_LEN+1];

  	while (total_sent < file_len &&  !file.eof()){
    	memset(request, 0, CHUNK_LEN+1);
    	file.read(request, CHUNK_LEN);
    
    	int request_len = CHUNK_LEN;
    	if (file_len - total_sent < CHUNK_LEN){
      		request_len = file_len - total_sent;
    	}	
    	bytes_sent = (this->socket).send_message(request, request_len);
    		
    	if (bytes_sent < 0){
    		break;
    	}
    	total_sent += bytes_sent;
  	}
}

void Protocol::send_size_first(unsigned int size){
	int digitos = get_digits(size);
	if(digitos > PROTOCOL_MSG_SIZE){
		throw Error("Mensaje demasiado largo!");
	}
	char msg_size[PROTOCOL_MSG_SIZE];
	memset(msg_size, 0, PROTOCOL_MSG_SIZE); 
	snprintf(msg_size,PROTOCOL_MSG_SIZE, "%d", size);
	 (this->socket).send_message(msg_size,PROTOCOL_MSG_SIZE);
}


int Protocol::receive_code(){
	char code[PROTOCOL_CODE];
	(this->socket).receive_message(code, PROTOCOL_CODE);
	return atoi(code);
}



std::string Protocol::receive_string(){
	ssize_t msg_size = receive_size_first();
	char chunk[CHUNK_LEN];
	(this->socket).receive_message(chunk, msg_size);
	chunk[msg_size] = 0;
	std::string string_received(chunk);
	return string_received;	
}


bool Protocol::receive_file(const std::string & name){
  if (receive_code() == NOT_OK){
    return false;
  }
	std::ofstream file(name,std::ofstream::out| std::ofstream::binary);
	ssize_t file_len = receive_size_first();
  char chunk[CHUNK_LEN+1];
	int total_received = 0;
	int bytes_received = 0;
	while (total_received < file_len){
      	memset(chunk, 0, CHUNK_LEN+1);

        int request_len = CHUNK_LEN;
        if (file_len - total_received < CHUNK_LEN){
            request_len = file_len - total_received;
        }
      	bytes_received = (this->socket).receive_message(chunk, request_len); 
        
      	total_received += bytes_received;
      	if (bytes_received <= 0){
        	break;
      	}
      	file.write(chunk, bytes_received);
    }
  return true; 
}

ssize_t Protocol::receive_size_first(){
	char msg_size[PROTOCOL_MSG_SIZE];
	(this->socket).receive_message(msg_size, PROTOCOL_MSG_SIZE);
	return atoi(msg_size);
}



