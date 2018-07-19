#include "commonSocket.h"


struct addrinfo * Socket::define_socket_num(struct addrinfo * node){
  	while (node != NULL) {
    	socket_num = //
      	socket(node->ai_family, //
      	node->ai_socktype, //
      	node->ai_protocol);
    	if (socket_num != -1) {
      		this->socket_num = socket_num;
      	 return node;
    	}	
    	node = node->ai_next;
  	}
    return NULL;	
}



void Socket::init(){
  	struct addrinfo *addrinfoNode = addrinfo();
  	if(define_socket_num(addrinfoNode)==NULL){
  		throw Error("Error en init\n");
  	}
  	free(addrinfoNode);	
}

struct addrinfo * Socket::addrinfo(){
  	struct addrinfo hints;
  	memset(&hints, 0, sizeof(struct addrinfo)); 
  	hints.ai_family = AF_INET;       //IPv4 
  	hints.ai_socktype = SOCK_STREAM; //TCP 
  	hints.ai_flags = 0; 

  	struct addrinfo *addrinfoNode;
    

  	int addrinfo_returnvalue = //
  	getaddrinfo(this->host_name, this->port, &hints, &addrinfoNode);

  	if (addrinfo_returnvalue != SUCCESS){
      gai_strerror(addrinfo_returnvalue);
  	}
  	return addrinfoNode;
}	

bool Socket::is_valid_port(const char * port){
	for (unsigned int i = 0; i < strlen(port); i ++){
		if (!isdigit(port[i])){
			return false;
		}
	}
	return true;
}




Socket::Socket(const char * host_name, const char * port){
  	if(!is_valid_port(port)){
  		throw Error("%s no es un purto valido\n"//
       "deben ser todos caracteres numéricos", port);
  	}

    this->host_name = host_name;
    this->port = port;
    this->is_connected = false;

    init();
    int val = 1;
    if (setsockopt(this->socket_num, SOL_SOCKET, SO_REUSEADDR, &val, //
      	sizeof(val)) == -1) {
        close(this->socket_num);
        throw Error("Error in reuse socket: %s\n", strerror(errno));
    }
}

Socket::Socket(int socket_num){
  this->is_connected = true;
  this->socket_num = socket_num;
}



Socket::Socket(Socket&& other){
  this->socket_num = other.socket_num;
  this->host_name = other.host_name;
  this->port = other.port;
  this->is_connected = other.is_connected;

  other.socket_num =-1;
  other.host_name ="null";
  other.port ="null";
  other.is_connected = false;
}
  
Socket& Socket::operator=(Socket&& other){
  if (this == &other) {
    return *this; // other is myself!
  }
  this->socket_num = other.socket_num;
  this->host_name = other.host_name;
  this->port = other.port;
  this->is_connected = other.is_connected;

  other.socket_num =-1;
  other.host_name ="null";
  other.port ="null";
  other.is_connected = false;
  return *this;
}


void Socket::connection(){
	struct addrinfo *addrinfoNode = addrinfo();

	struct addrinfo *node  = define_socket_num(addrinfoNode);
  
  while (node != NULL && this->is_connected == false) {
  		int connection_returnvalue = connect(this->socket_num, //
      	node->ai_addr, node->ai_addrlen);
    	this->is_connected = (connection_returnvalue != -1);
    	if (this->is_connected){
      		break;
    	}
    	node = define_socket_num(addrinfoNode);
  }
  freeaddrinfo(addrinfoNode);
  if (this->is_connected == false) {
    	close(this->socket_num);
      throw Error("Error in socket connection: %s\n", strerror(errno));
  }
}

void Socket::bind_and_listen(){
  	struct addrinfo *addrinfoNode = addrinfo();

  	int bindReturnValue = -1;
  	while (addrinfoNode != NULL) {
    	bindReturnValue = bind(this->socket_num, //
    	addrinfoNode->ai_addr, //
    	addrinfoNode->ai_addrlen);
    	if (bindReturnValue != -1) {
      		break;
   	 	}
    	addrinfoNode = addrinfoNode->ai_next;
  	}
  	free(addrinfoNode);

  	if (bindReturnValue == -1) {
    	throw Error("Error in bind: %s\n", strerror(errno));
  	}
  	int listenReturnValue = listen(this->socket_num, 20);
  	if (listenReturnValue == -1) {
    	throw Error("Error in listen: %s\n", strerror(errno));
  	}
  	this-> is_connected = true;
}

Socket Socket::accept_socket(){
  	int new_sockfd = accept(this->socket_num, NULL, NULL);
  	if (new_sockfd == -1){
    	throw Error("Error in accept");
  	}
  	return std::move(Socket(new_sockfd));
}

int Socket::receive_message(char* buffer, const size_t size){
    int total_received = 0;
    int bytes_recived = 0;


    while ((bytes_recived = recv(this->socket_num, //
    &buffer[total_received],//
    size - total_received, MSG_NOSIGNAL)) >0) {
      total_received += bytes_recived;
      if (size -total_received== 0){
        break;
      }
    }
    if (bytes_recived < 0) { 
      throw Error(" Error recibing info: %s\n", strerror(errno));
    }
    return total_received;
}


int Socket::send_message(const char* buffer,const size_t size){
    int total_sent = 0;
    int bytes_sent = 0;

    while ((bytes_sent = send(this->socket_num, //
      &buffer[total_sent], size-total_sent, MSG_NOSIGNAL)) >0){
        total_sent += bytes_sent;
    }
    if (bytes_sent < 0) { 
      throw Error(" Error sending info: %s\n", strerror(errno));
    }
    return total_sent;
}

void Socket::stop(){
  shutdown(this->socket_num, SHUT_RDWR);
  close(this->socket_num);
}

Socket::~Socket(){
  if (this->socket_num > 0){
    shutdown(this->socket_num, SHUT_RDWR);
    close(this->socket_num);
  } 
}

