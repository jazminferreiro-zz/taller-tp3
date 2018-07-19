#include "serverThConnection.h"

void Thread::start() {
    thread = std::thread(&Thread::run, this);
}

void Thread::join() {
    thread.join();
}

ThConnection::ThConnection(Socket peer, Index & index)://
 connection(std::move(peer), index){}

bool ThConnection::is_dead() {
	return this->dead;
}
void ThConnection::run() {
    dead = false;
    this->connection.run();
    dead = true;   
}

void ThConnection::stop() {
	this->connection.stop();
}







