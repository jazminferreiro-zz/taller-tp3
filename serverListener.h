#include "serverConnection.h"
#include "serverThConnection.h"
#include <thread>
#include <vector>

class Server_listener{
private:
	Socket socket;
	Index index;
	std::thread principal_thread;
	std::vector<ThConnection*> client_threads;
	bool continue_listening;
	void listen();
public:
	//constructor que crea un socket de servidor
	Server_listener(char port[], char file_name[]);

	//comienza a escuchar conecciones
	void start_listening();

	//finaliza las conecciones
	void stop_listening();	
};
