#ifndef  CTSERVER_H
#define  CTSERVER_H

#include <string>
#include <time.h>

#include <zmq.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include <ctCore/ctCore.h>
#include <ctCore/ctConstants.h>

using namespace std;
using namespace zmq;
using namespace boost::posix_time;

enum ctServerState
{
	CT_STATE_NotStarted,
	CT_STATE_Started,
	CT_STATE_Finished
};

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

		void createPublisherSocket();
		
		bool init();

		void startTimer();

		string getRemainingTimeAsString();

		void handleIncoming();

		void tick();

		string getStateAsString();

		void switchState(ctServerState newState);

		bool isTimerFinished();

		void publishOutgoing();

	protected:
		context_t* m_PublisherContext;
		context_t* m_ResponderContext;

		socket_t* m_Publisher;
		socket_t* m_Responder;

		string m_PublisherAddr;
		string m_ResponderAddr;

		ptime m_StartTime;
		ptime m_EndTime;

		ctServerState m_CurrentState;

	private:
};


#endif   // CTSERVER_H
