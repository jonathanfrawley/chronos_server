#ifndef  CTCOMMANDLINECLIENT_H
#define  CTCOMMANDLINECLIENT_H

#include <string>
#include <stdio.h>
#include <zmq.hpp>
#include <time.h>

#include <ctCore/ctCore.h>
#include <ctCore/ctConstants.h>

using namespace std;
using namespace zmq;

/*
 * =====================================================================================
 *        Class: ctCommandLineClient
 *  Description:
 * =====================================================================================
 */
class ctCommandLineClient
{
	public:
		ctCommandLineClient(string subAddr, string senderAddr);

		virtual ~ctCommandLineClient();

		bool init();

		void assertRcType(int rc);

		void mainLoop();

		void sendProtocol(char protocol);

		bool isServerUp();

		bool listenToPublisher();
		
	protected:
		context_t* m_Context;
		socket_t* m_Sender;
		socket_t* m_Subscriber;

		string m_SenderAddr;
		string m_SubAddr;
	private:
};


#endif   // CTCOMMANDLINECLIENT_H
