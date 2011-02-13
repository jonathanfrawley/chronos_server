#include "ctServer.h"

#include <stdio.h>

ctServer::ctServer(string publisherAddr, string responderAddr)
	:
		m_PublisherAddr(publisherAddr),
		m_ResponderAddr(responderAddr),
		m_Publisher(0),
		m_Responder(0),
		m_PublisherContext(0),
		m_ResponderContext(0)
{
}

ctServer::~ctServer()
{
	CT_SAFE_DELETE(m_PublisherContext);
	CT_SAFE_DELETE(m_ResponderContext);
	CT_SAFE_DELETE(m_Publisher);
	CT_SAFE_DELETE(m_Responder);
}

void ctServer::createResponderSocket()
{
	m_ResponderContext = CT_NEW context_t(CT_N_SERVER_THREADS);
	m_Responder = CT_NEW socket_t(*m_ResponderContext, ZMQ_REP);
//	m_Responder->bind (m_ResponderAddr.c_str());
	m_Responder->bind ("tcp://*:5555");
}

bool ctServer::init()
{
//	m_PublisherContext = CT_NEW context_t(CT_N_SERVER_THREADS);

//	m_Publisher = CT_NEW socket_t(*m_PublisherContext, ZMQ_PUB);

//    m_Publisher->connect (m_PublisherAddr.c_str());

	createResponderSocket();

	return true;
}

void ctServer::handleIncoming()
{

	message_t request;

	//  Wait for next request from client
	m_Responder->recv (&request);
	printf ("Received request: [%s]\n",
			(char *) request.data ());

	//Handle request
	char protocol = ((char*)request.data())[0];
	if(protocol == CT_START)
	{
		//  Send reply back to client
		message_t reply(11);
		memcpy ((void *) reply.data(), "Starting..", 11);
		m_Responder->send(reply);
	}
}

void ctServer::tick()
{
	handleIncoming();
}
