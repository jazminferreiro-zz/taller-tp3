#include "error.h"
#include "client.h"

#define SERVER_NAME 1 
#define PORT 2


int main(int argc, char * argv[]){
	if (argc < 4){
		throw Error("Parametros incorrectos");
	}
		
	Client cliente(argv[SERVER_NAME],argv[PORT]);
	cliente.execute_command(argc, argv);
	return 0;
}

