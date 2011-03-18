#ifndef  CTCOMMANDLINECLIENT_H
#define  CTCOMMANDLINECLIENT_H

#include <string>
#include <stdio.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <ctCore/ctCore.h>
#include <ctCore/ctConstants.h>

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
inline void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

using namespace std;

/*
 * =====================================================================================
 *        Class: ctCommandLineClient
 *  Description:
 * =====================================================================================
 */
class ctCommandLineClient
{
	public:
		ctCommandLineClient(string serverAddr, string serverPort);

		virtual ~ctCommandLineClient();

		bool init();

		void mainLoop();

		void sendProtocol(char protocol);

	protected:
		string m_ServerAddr, m_ServerPort;

		int sockfd, numbytes;  
		char buf[MAXDATASIZE];
		struct addrinfo hints, *servinfo, *p;
		int rv;
		char s[INET6_ADDRSTRLEN];
	private:
};


#endif   // CTCOMMANDLINECLIENT_H
