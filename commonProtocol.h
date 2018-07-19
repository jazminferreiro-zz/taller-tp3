#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__


#include "commonSocket.h"


#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>


using std::cout;
using std::endl;
using std::string;


#define CHUNK_LEN 64


#define PROTOCOL_MSG_SIZE 4
#define PROTOCOL_CODE 2
#define NO_PEER_SKT -1

#define PUSH_CODE 1
#define TAG_CODE 2
#define PULL_CODE 3

#define OK 1	
#define NOT_OK 0



class Protocol{
private:
int get_digits(unsigned int num){
    int digits = 1;
    while ( num > 0 ) {
        num /= 10;
        digits++;
    }
    return digits;
}



protected:
	Socket socket;
	explicit Protocol(Socket socket);


	//envia en un solo byte un numero
	void send_code(int code_to_send);

	//envia un string
	//siempre antes enviando el tamaño
	void send_string(const string & string_to_send);

	//envia un archivo de a chunks
	//siempre antes enviando el tamaño total
	void send_file(const std::string & file_name);

	//envia 4 bytes con el tamaño
	void send_size_first(unsigned int size);


	//recibe un solo byte con un numero
	int receive_code();

	//recibe primero la longitud y luego un string
	std::string receive_string();

	//recibe primero la longitud total y
	//luego el archivo de a chunks
	bool receive_file(const std::string & hash);
		

	//recibe el tamaño en 4 bytes
	ssize_t receive_size_first();
};

#endif

