#ifndef  CTSERVER_H
#define  CTSERVER_H

#include <string>
#include <time.h>

#include <pthread.h>

//#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include <ctCore/ctCore.h>
#include <ctCore/ctConstants.h>


using namespace std;
//using namespace zmq;
using namespace boost::posix_time;

const int BACKLOG = 10;

enum ctServerState
{
	CT_STATE_NotStarted,
	CT_STATE_Started,
	CT_STATE_Finished
};

inline void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
inline void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/*
 * =====================================================================================
 *        Class: ctServer
 *  Description:
 * =====================================================================================
 */
class ctServer
{
	public:
		ctServer(string port);

		virtual ~ctServer();

		/*
		void createResponderSocket();

		void createPublisherSocket();
		*/
		
		bool init();

		void startTimer();

		string getRemainingTimeAsString();

//		void handleIncoming();

		void tick();

		string getStateAsString();

		void switchState(ctServerState newState);

		bool isTimerFinished();

		void handleProtocol(int protocol, int fd);

		void publishOutgoing();

	protected:
		/*
		context_t* m_PublisherContext;
		context_t* m_ResponderContext;

		socket_t* m_Publisher;
		socket_t* m_Responder;
		*/

//		string m_PublisherAddr;
//		string m_ResponderAddr;

		string m_Port;

		ptime m_StartTime;
		ptime m_EndTime;

		ctServerState m_CurrentState;

		int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
		struct addrinfo hints, *servinfo, *p;
		struct sockaddr_storage their_addr; // connector's address information
		socklen_t sin_size;
		struct sigaction sa;
		int yes;
		char s[INET6_ADDRSTRLEN];
		int rv;

	private:
};

typedef struct _serverThreadBundle
{
	int fd;
	ctServer* server;
} serverThreadBundle;

inline void* launchHandleProtocolFunction(void* bundle)
{
	serverThreadBundle* castBundle;
    castBundle = (serverThreadBundle*)bundle;
	ctServer* server = castBundle->server;

	char buf[0];
	if( recv(castBundle->fd, buf, 1, 0) == -1 )
	{
		perror("receive");
	}
	int protocol = (int)buf[0];

	cout<<"Handling protocol : "<<protocol<<endl;

	server->handleProtocol(protocol, castBundle->fd);
//    return ((Server *)obj)->processRequest();
	return NULL;
}

#endif   // CTSERVER_H
