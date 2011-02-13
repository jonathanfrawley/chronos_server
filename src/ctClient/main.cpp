//
//  chronos client
//
#include <ctClient/ctCommandLineClient.h>
#include <string>

using namespace std;

int main () {
	ctCommandLineClient* client = new ctCommandLineClient(string("tcp://localhost:10000"));
	client->init();
	client->mainLoop();

	delete client;

    return 0;
}

