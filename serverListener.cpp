#include "serverListener.h"
#include <vector>
Server_listener::Server_listener(char port[], char file_name[])://
 socket(NULL,port), index(file_name){
	this->continue_listening = true;
	this->socket.bind_and_listen();
}

void Server_listener::start_listening(){
	this->principal_thread = std::thread(&Server_listener::listen, this);
} 	

void Server_listener::listen(){
	while(this->continue_listening){
		try{
			Socket peer = this->socket.accept_socket(); 	
			client_threads.push_back(new ThConnection(std::move(peer), this->index));
			client_threads.back()->start();
		} catch(Error e){ 
			//cerraron el socket
		}
    }
}

void Server_listener::stop_listening(){
	this->continue_listening = false;
	
	this->socket.stop();

	for(unsigned int i = 0; i< client_threads.size(); i++){
		client_threads[i]->stop();
		client_threads[i]->join();
		delete client_threads[i];
	}

    this->principal_thread.join();
    this->index.close();
}	
