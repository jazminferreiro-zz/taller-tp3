
#define PORT 1
#define INDEX_FILE 2 

#include "serverListener.h"
#include <thread>


int main(int argc, char * argv[]){
	if(argc < 2){
		throw Error("Parametros incorrectos");
	}

	Server_listener s(argv[PORT],argv[INDEX_FILE]);
	s.start_listening();

	char c = getchar();
	while(c != 'q'){
		c = getchar();
	}

	s.stop_listening();
	
	return 0;
}
