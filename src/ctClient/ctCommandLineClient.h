#ifndef  CTCOMMANDLINECLIENT_H
#define  CTCOMMANDLINECLIENT_H

#include <string>
#include <stdio.h>
#include <zmq.hpp>

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
		ctCommandLineClient(string senderAddr);

		virtual ~ctCommandLineClient();

		bool init();

		void assertRcType(int rc);

		void mainLoop();

		void sendProtocol(char protocol);
		
	protected:
		context_t* m_Context;
		socket_t* m_Sender;

		string m_SenderAddr;
	private:
};


#endif   // CTCOMMANDLINECLIENT_H
