//
//  chronos server
//  Binds REP socket to tcp://*:5555
//
#include <pthread.h>

#include <ctServer/ctServer.h>

ctServer* server;

typedef struct publishThreadBundle
{
	ctServer* server;
} publishThreadBundle;

typedef struct listenThreadBundle
{
	ctServer* server;
} listenThreadBundle;

void* publishThreadFunc(void* threadBundle)
{
	publishThreadBundle* bundle = (publishThreadBundle*)threadBundle;
	while(1)
	{
		bundle->server->publishOutgoing();
		sleep(1);
	}
}

void* listenThreadFunc(void* threadBundle)
{
	listenThreadBundle* bundle = (listenThreadBundle*)threadBundle;
	while(1)
	{
		bundle->server->handleIncoming();
	}
}

int main () {
	server = new ctServer("tcp://*:10001", "tcp://*:10000");
	server->init();
	/*
	   while(true)
	   {
	   server->tick();
	   }
	   */
	pthread_t threads[2];
	publishThreadBundle* bundle = (publishThreadBundle*) malloc(sizeof(publishThreadBundle));
	bundle->server = server;
	pthread_create(&threads[0], NULL, publishThreadFunc, (void*)bundle);

	listenThreadBundle* lbundle = (listenThreadBundle*) malloc(sizeof(listenThreadBundle));
	lbundle->server = server;
	pthread_create(&threads[1], NULL, listenThreadFunc, (void*)lbundle);

	pthread_exit(NULL);

	delete server;

    return 0;
}

