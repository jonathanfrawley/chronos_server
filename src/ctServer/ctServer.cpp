#include "ctServer.h"

#include <stdio.h>

ctServer::ctServer(string port)
	:
		m_Port(port),
		yes(1)
{
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

}

ctServer::~ctServer()
{
}

bool ctServer::init()
{
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
	m_EndTime = endTime;
}

string ctServer::getRemainingTimeAsString()
{
	ptime now = second_clock::local_time();
	time_duration remaining = m_EndTime - now;

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
		string timeString = getRemainingTimeAsString();
		char reply[20];
		memcpy ((void *) reply, timeString.c_str(), strlen(timeString.c_str()));
		if (send(fd, reply, strlen(timeString.c_str()), 0) == -1)
		{
			perror("send");
		}
	}
}

void ctServer::tick()
{
	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1) {
		perror("accept");
		return;
	}

	inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);

	serverThreadBundle bundle;
	bundle.fd = new_fd;
	bundle.server = this;

	pthread_t t;
	pthread_create(&t, 0, launchHandleProtocolFunction, (void*)&bundle);

	pthread_join(t, NULL);
	close(new_fd);  // parent doesn't need this
}
