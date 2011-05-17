//
//  chronos client
//

#include <iostream>
#include <boost/program_options.hpp>

#include <ctClient/ctCommandLineClient.h>
#include <string>

using namespace std;

int main (int argc, char** argv) 
{
	ctCommandLineClient* client = new ctCommandLineClient("localhost",string("10000"));
	if(client->init())
	{
		char method;
		//Program options
		namespace po = boost::program_options;
		po::options_description desc("Allowed Options");
		desc.add_options()
			("help", "produce help message")
			("m", po::value<char>(), "mode. \'b\' to start timer, \'s\' for state of server, and \'c\' to check the amount of time left.")
			;

		po::variables_map vmCmdLine;
		store(po::parse_command_line(argc, argv, desc), vmCmdLine);
		po::notify(vmCmdLine);

		if (vmCmdLine.count("help")) 
		{
			cout << desc << "\n";
			return 1;
		}

		if (vmCmdLine.count("m"))
		{
			method = vmCmdLine["m"].as< char >();
			client->sendProtocol(method);
		}

		//	client->mainLoop();
	}

	delete client;

	return 0;
}

