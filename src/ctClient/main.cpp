//
//  chronos client
//

#include <iostream>
#include <boost/program_options.hpp>

#include <ctClient/ctCommandLineClient.h>
#include <string>

using namespace std;

int main (int argc, char** argv) {
	ctCommandLineClient* client = new ctCommandLineClient("tcp://localhost:10001",string("tcp://localhost:10000"));
	client->init();

	char method;
	//Program options
	namespace po = boost::program_options;
	po::options_description desc;
	desc.add_options()
		("-m", po::value<char>(), "\'b\' to start timer, \'s\' for state of server, and \'c\' to check the amount of time left.")
		;
	po::variables_map vmCmdLine;
	store(po::parse_command_line(argc, argv, desc), vmCmdLine);
	if (vmCmdLine.count("-m"))
	{
		method = vmCmdLine["-m"].as< char >();
		if(method == 'c')
		{
			client->listenToPublisher();
		}
		else
		{
			client->sendProtocol(method);
		}
	}

//	client->mainLoop();

	delete client;

    return 0;
}

