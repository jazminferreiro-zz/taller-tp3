#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <iostream>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include "commonError.h"


#define SUCCESS 0
#define ERROR 1

class Socket {
  private:
    int socket_num;
    const char * host_name;
    const char * port;
    bool is_connected;
    
  //data una estructura del tipo addrinfo, la recorre hasta 
  //poder crear un socket y setea entonces el numero del socket
  struct addrinfo * define_socket_num(struct addrinfo * node);

  //crea un socket inicial
  void init();

  //devuelve el primer nodo de addrinfo
  struct addrinfo * addrinfo();

  //valida que el puerto sea valido
  bool is_valid_port(const char * port);



public:
  //Constructor por copia anulado
  Socket(const Socket& other) = delete;
  //Asignacion por copia anulada
  Socket& operator=(const Socket &other) = delete;
  //Constructor por movimiento anulado
  Socket(Socket&& other); 
  //Asignacion por movimiento anulada
  Socket& operator=(Socket&& other);

  //constructor del socket
  Socket(const char * host_name, const char * port);

  //constructor del peer socket ya conectado
  explicit Socket(int socket_num);

  //trata de conectar al socket
  void connection();

  //sirve para servidores, 
  //el socket se quedara esperando que se conecten con el 
  void bind_and_listen();

  //sirve para servidores,
  //acepta a otro socket que quiera conectarse con el
  Socket accept_socket();

  //Debe estar conectado con el socket cuyo numero sea skt_num
  //porque a partir de él recibirá datos
  //Tratará de recibir tantos  datos como se especifique
  //en el parametro size.
  //guarda los datos recibidos en buffer que debe ser un array
  //de un tamaño mayor  o igual a size
  int receive_message(char* buffer, size_t size);


  
  //Tratará de enviar tantos datos como se especifique
  //en el parámetro size.
  //enciará los datos en buffer que debe ser un array
  //de un tamaño mayor o igual a size
  int send_message(const char* buffer, size_t size);

  void stop();

  //destructor del socket
  ~Socket();
};

#endif
