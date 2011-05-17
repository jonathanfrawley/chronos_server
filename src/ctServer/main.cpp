//
//  chronos server
//  Binds REP socket to tcp://*:5555
//
#include <pthread.h>

#include <ctServer/ctServer.h>
int main () {
	ctServer* server = new ctServer("10000");
	server->init();
	while(true)
	{
		server->tick();
	}

	delete server;

    return 0;
}

