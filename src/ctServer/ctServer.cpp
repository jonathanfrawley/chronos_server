#include "ctServer.h"

#include <stdio.h>

ctServer::ctServer(string port)
	:
		m_Port(port),
		yes(1)
//		m_PublisherAddr(publisherAddr),
//		m_ResponderAddr(responderAddr)
		/*
		m_Publisher(0),
		m_Responder(0),
		m_PublisherContext(0),
		m_ResponderContext(0)
		*/
{
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

}

ctServer::~ctServer()
{
//	CT_SAFE_DELETE(m_PublisherContext);
//	CT_SAFE_DELETE(m_ResponderContext);
//	CT_SAFE_DELETE(m_Publisher);
//	CT_SAFE_DELETE(m_Responder);
}

/*
void ctServer::createResponderSocket()
{
	m_ResponderContext = CT_NEW context_t(CT_N_SERVER_THREADS);
	m_Responder = CT_NEW socket_t(*m_ResponderContext, ZMQ_REP);
	m_Responder->bind (m_ResponderAddr.c_str());
}

void ctServer::createPublisherSocket()
{
	m_PublisherContext = CT_NEW context_t(CT_N_SERVER_THREADS);

	m_Publisher = CT_NEW socket_t(*m_PublisherContext, ZMQ_PUB);

    m_Publisher->bind (m_PublisherAddr.c_str());
}
*/

bool ctServer::init()
{
/*
	createResponderSocket();

	createPublisherSocket();
*/

    if ((rv = getaddrinfo(NULL, m_Port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return false;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            return false;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return false;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
		return false;
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
		return false;
    }
	return true;
}

void ctServer::startTimer()
{
	ptime now = second_clock::local_time();
	m_StartTime = now;
	ptime endTime = second_clock::local_time();
	endTime = now + minutes(25);
//	endTime = now + seconds(15);
	m_EndTime = endTime;

//	cout<<"time now is "<<to_simple_string(now)<<endl;
//	cout<<"m_EndTime "<<to_simple_string(m_EndTime)<<endl;
//	cout<<"endtime "<<to_simple_string(endTime)<<endl;
}

string ctServer::getRemainingTimeAsString()
{
	ptime now = second_clock::local_time();
	time_duration remaining = m_EndTime - now;

//	cout<<"time now is "<<to_simple_string(now)<<endl;
//	cout<<"m_EndTime "<<to_simple_string(m_EndTime)<<endl;
//	cout<<"remaining "<<to_simple_string(remaining)<<endl;

	return to_simple_string(remaining);
}

string ctServer::getStateAsString()
{
	switch(m_CurrentState)
	{
		case CT_STATE_NotStarted:
			return "CT_STATE_NotStarted";
		case CT_STATE_Started:
			return "CT_STATE_Started";
		case CT_STATE_Finished:
			return "CT_STATE_Finished";
		default:
			return "Unknown state";
	}
}

void ctServer::switchState(ctServerState newState)
{
	m_CurrentState = newState;
}

bool ctServer::isTimerFinished()
{
	ptime now = second_clock::local_time();
	if(m_EndTime < now)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
void ctServer::handleIncoming()
{
	message_t request;

	//  Wait for next request from client
//	m_Responder->recv (&request, ZMQ_NOBLOCK);
	m_Responder->recv (&request);
//	printf ("Received request: [%c]\n",
//			((char *) request.data ())[0] );

	if(request.size() == 0)
	{
		return;
	}

	//Handle request
	char protocol = ((char*)request.data())[0];

	if(protocol == CT_START)
	{
//		cout<<"Start protocol here"<<endl;
		switchState(CT_STATE_Started);
		startTimer();
		//  Send reply back to client
		message_t reply(11);
		memcpy ((void *) reply.data(), "Starting..", 11);
		m_Responder->send(reply);
	}
	else if(protocol == CT_CHECK)
	{
		if(isTimerFinished())
		{
			switchState(CT_STATE_Finished);
		}
//		cout<<"Check protocol here"<<endl;
		string timeString = getRemainingTimeAsString();
		message_t reply(strlen(timeString.c_str()));
		memcpy ((void *) reply.data(), timeString.c_str(), strlen(timeString.c_str()));
		m_Responder->send(reply);
	}
	else if(protocol == CT_STATE)
	{
//		cout<<"State protocol here"<<endl;
		string stateString = getStateAsString();
		message_t reply(strlen(stateString.c_str()));
//		cout <<"Sending : " <<stateString<<endl;
		memcpy ((void *) reply.data(), stateString.c_str(), strlen(stateString.c_str()));
		m_Responder->send(reply);
	}
	else
	{
		assert(0 && "Null Packet");
	}
}

void ctServer::publishOutgoing()
{
	string timeString = getRemainingTimeAsString();
	message_t message(strlen(timeString.c_str()));
	memcpy ((void *) message.data(), timeString.c_str(), strlen(timeString.c_str()));
	m_Publisher->send(message);
}
*/
void ctServer::handleProtocol(int protocol, int fd)
{
	if(protocol == CT_START)
	{
//		cout<<"Starting timer.."<<endl;
		switchState(CT_STATE_Started);
		startTimer();
		//  Send reply back to client
		char reply[11];
		memcpy ((void *) reply, "Starting..", 11);
		if (send(fd, reply, 11, 0) == -1)
		{
			perror("send");
		}
	}
	else if(protocol == CT_CHECK)
	{
		if(isTimerFinished())
		{
			switchState(CT_STATE_Finished);
		}
//		cout<<"Check protocol here"<<endl;
		string timeString = getRemainingTimeAsString();
		char reply[20];
		memcpy ((void *) reply, timeString.c_str(), strlen(timeString.c_str()));
		if (send(fd, reply, strlen(timeString.c_str()), 0) == -1)
		{
			perror("send");
		}
		/*
		message_t reply(strlen(timeString.c_str()));
		memcpy ((void *) reply.data(), timeString.c_str(), strlen(timeString.c_str()));
		m_Responder->send(reply);
		*/
	}
	/*
	else if(protocol == CT_STATE)
	{
//		cout<<"State protocol here"<<endl;
		string stateString = getStateAsString();
		message_t reply(strlen(stateString.c_str()));
//		cout <<"Sending : " <<stateString<<endl;
		memcpy ((void *) reply.data(), stateString.c_str(), strlen(stateString.c_str()));
		m_Responder->send(reply);
	}
	else
	*/
}

void ctServer::tick()
{
	//handleIncoming();
	//checkForTimerUp();
	//publishOutgoing();
	//sleep(1);
	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1) {
		perror("accept");
		return;
	}

	inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
//	printf("server: got connection from %s\n", s);

	//TODO : Change multiprocess to multithreaded and have timer and state as mutexed variables
	serverThreadBundle bundle;
	bundle.fd = new_fd;
	bundle.server = this;

	pthread_t t;
	pthread_create(&t, 0, launchHandleProtocolFunction, (void*)&bundle);

	pthread_join(t, NULL);


/*
	if (!fork()) { // this is the child process
		close(sockfd); // child doesn't need the listener

		char buf[0];
		if( recv(new_fd, buf, 1, 0) == -1 )
		{
			perror("receive");
		}

		printf("received : <%c> \n", buf[0]);
		int protocol = (int)(buf[0]);
		handleProtocol(protocol, new_fd);

		close(new_fd);
		exit(0);
	}
*/
	close(new_fd);  // parent doesn't need this

}
