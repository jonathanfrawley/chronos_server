#include "ctCommandLineClient.h"

#include <iostream>

ctCommandLineClient::ctCommandLineClient(string senderAddr)
	:
		m_SenderAddr(senderAddr)
{
}

ctCommandLineClient::~ctCommandLineClient()
{
	CT_SAFE_DELETE(m_Context);
	CT_SAFE_DELETE(m_Sender);
}

bool ctCommandLineClient::init()
{
	m_Context = CT_NEW context_t(CT_N_CLIENT_THREADS);

	m_Sender = CT_NEW socket_t(*m_Context, ZMQ_REQ);

	try
	{
	//	m_Sender->connect (m_SenderAddr.c_str());
		m_Sender->connect ("tcp://localhost:5555");
	}
	catch(exception e)
	{
		assert(0);
	}

	return true;
}

void ctCommandLineClient::assertRcType(int rc)
{
	switch(rc)
	{
		case EAGAIN:
			{
			assert(0 && "EAGAIN");
			break;
			}
		case ENOTSUP:
			{
			assert(0 && "ENOTSUP");
			break;
			}
		case EFSM:
			{
			assert(0 && "EFSM");
			break;
			}
		case EFAULT:
			{
			assert(0 && "EFAULT");
			break;
			}
		default:
			{
			break;
			}
	}
}

void ctCommandLineClient::sendProtocol(char protocol)
{
	char buf[CT_MAX_PACKET_SIZE];
	buf[0] = protocol;
	message_t* message = CT_NEW message_t(buf, CT_MAX_PACKET_SIZE, NULL);

	try
	{
		m_Sender->send (*message);
	}
	catch(exception& e)
	{
		cout<<e.what()<<endl;
		assert(0);
	}
	delete message;

	message_t reply(CT_MAX_PACKET_SIZE);
	int rc = m_Sender->recv(&reply);
	if(reply.size() == 0)
	{
		//A null packet received
		//assert(0 && "null packet received");
		return;
	}
	printf ("Received reply : [%s]\n",
			(char *) (reply.data()));

}

void ctCommandLineClient::mainLoop()
{
	sendProtocol((char)CT_START);
}
