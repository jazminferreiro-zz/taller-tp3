#include "Actioner.h"


Actioner::Actioner(Socket & socket, BlockingQueue<ActionDTO> queue):
	socket(socket),
	queue(queue){
	this->on = true;
}

void Actioner::start(){
	this->principal_thread = std::thread(&Actioner::make_action, this);
} 	

void Actioner::make_action(){
	while(this->on){
		
    }
}

void Actioner::stop(){
	this->on= false;
}	
