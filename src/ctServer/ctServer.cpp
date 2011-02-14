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
	m_Responder->bind (m_ResponderAddr.c_str());
}

void ctServer::createPublisherSocket()
{
	m_PublisherContext = CT_NEW context_t(CT_N_SERVER_THREADS);

	m_Publisher = CT_NEW socket_t(*m_PublisherContext, ZMQ_PUB);

    m_Publisher->bind (m_PublisherAddr.c_str());
}

bool ctServer::init()
{

	createResponderSocket();

	createPublisherSocket();

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

void ctServer::handleIncoming()
{
	message_t request;

	//  Wait for next request from client
	m_Responder->recv (&request, ZMQ_NOBLOCK);
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

void ctServer::tick()
{
	handleIncoming();
	//checkForTimerUp();
	publishOutgoing();
}
