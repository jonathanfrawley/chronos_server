#include "ctCommandLineClient.h"

#include <iostream>

ctCommandLineClient::ctCommandLineClient(string subAddr, string senderAddr)
	:
		m_SenderAddr(senderAddr),
		m_SubAddr(subAddr)
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

	m_Subscriber = CT_NEW socket_t(*m_Context, ZMQ_SUB);
	m_Subscriber->setsockopt (ZMQ_SUBSCRIBE, 0, 0); //Subscribe to all messages? Maybe make more efficient.
	
	try
	{
		m_Sender->connect (m_SenderAddr.c_str());
		m_Subscriber->connect (m_SubAddr.c_str());
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
		m_Sender->send (*message, ZMQ_NOBLOCK);
	}
	catch(exception& e)
	{
		cout<<e.what()<<endl;
//		assert(0);	
		delete message;
	}
	delete message;

	message_t* reply = CT_NEW message_t(CT_MAX_PACKET_SIZE);
	int rc = m_Sender->recv(reply);

	if(reply->size() == 0)
	{
		//A null packet received
		assert(0 && "null packet received");
		return;
	}
//	printf ("Received reply : [%s]\n",
//			(char *) (reply->data()));

	printf ("%s\n",
			(char *) (reply->data()));

}

//Determines whether server is up by trying to send a req
bool ctCommandLineClient::isServerUp()
{
	char buf[CT_MAX_PACKET_SIZE];
	buf[0] = 's';
	message_t* message = CT_NEW message_t(buf, CT_MAX_PACKET_SIZE, NULL);

	try
	{
		m_Sender->send (*message, ZMQ_NOBLOCK);
	}
	catch(exception& e)
	{
		cout<<e.what()<<endl;
		delete message;
		assert(0);	
	}
	delete message;

	int nRetries = 2;
	for(int i = 0 ; i < nRetries ; i++)
	{
		message_t* reply = CT_NEW message_t(CT_MAX_PACKET_SIZE);
		int rc = m_Sender->recv(reply, ZMQ_NOBLOCK);

		if(reply->size() == 0)
		{
			ct_delay(10);
			continue;
		}
		else
		{
			ct_delay(10*(nRetries-i));
			return true;
		}
	}
	return false;
}

bool ctCommandLineClient::listenToPublisher()
{
	message_t* message = CT_NEW message_t();

	/*
	m_Subscriber->recv(message, ZMQ_NOBLOCK);
	if(message->size() == 0)
	{
		//A null packet received
		return;
	}
	printf ("%s\n",
			(char *) (message->data()));
	return;
	*/

	if(isServerUp())
	{
		m_Subscriber->recv(message);
		if(message->size() == 0)
		{
			//A null packet received
			return false;
		}
		printf ("%s\n",
				(char *) (message->data()));
		return true;
	}
	else
	{
		return false;
	}

	/*
	//TODO:Set this to a sensible value 
	for(int i = 0 ; i < 10000000 ; i++)
	{
		m_Subscriber->recv(message, ZMQ_NOBLOCK);
	//	m_Subscriber->recv(message);
		if(message->size() == 0)
		{
			//A null packet received
			continue;
		}
		printf ("%s\n",
				(char *) (message->data()));
		return;
	}
	*/
}

void ctCommandLineClient::mainLoop()
{
	sendProtocol((char)CT_START);

	sendProtocol((char)CT_CHECK);

	sendProtocol((char)CT_STATE);
}
