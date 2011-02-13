#ifndef  CTSERVER_H
#define  CTSERVER_H

#include <string>
#include <zmq.hpp>

#include <ctCore/ctCore.h>
#include <ctCore/ctConstants.h>

using namespace std;
using namespace zmq;

/*
 * =====================================================================================
 *        Class: ctServer
 *  Description:
 * =====================================================================================
 */
class ctServer
{
	public:
		ctServer(string publisherAddr, string responderAddr);

		virtual ~ctServer();

		void createResponderSocket();
		
		bool init();

		void handleIncoming();

		void tick();

	protected:
		context_t* m_PublisherContext;
		context_t* m_ResponderContext;

		socket_t* m_Publisher;
		socket_t* m_Responder;

		string m_PublisherAddr;
		string m_ResponderAddr;

	private:
};


#endif   // CTSERVER_H
