#include "ctCommandLineClient.h"

#include <iostream>

ctCommandLineClient::ctCommandLineClient(string serverAddr, string serverPort)
	:
		m_ServerAddr(serverAddr),
		m_ServerPort(serverPort)
{
}

ctCommandLineClient::~ctCommandLineClient()
{
}

bool ctCommandLineClient::init()
{
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(m_ServerAddr.c_str(), m_ServerPort.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return false;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return false;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

	return true;
}

void ctCommandLineClient::sendProtocol(char protocol)
{
	char protocolBuf[1];
	protocolBuf[0] = protocol;
	if (send(sockfd, protocolBuf, 1, 0) == -1)
	{
        perror("send");
        exit(1);
	}

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);

    close(sockfd);
}

void ctCommandLineClient::mainLoop()
{
	sendProtocol((char)CT_START);

	sendProtocol((char)CT_CHECK);

//	sendProtocol((char)CT_STATE);
}
