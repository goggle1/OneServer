#include <sys/prctl.h>
#include <sys/epoll.h>

#include "EventHandler.h"
#include "EventThread.h"

EventThread::EventThread()
{
	m_events.Init();
}

EventThread::~EventThread()
{
}

int EventThread::Entry()
{
	prctl(PR_SET_NAME, "oneserver_events");
	
	struct epoll_event events[MAX_EVENTS];
	while(1)
	{		
		int num = epoll_wait(m_events.m_epoll_fd, events, MAX_EVENTS, WAIT_PERIOD);
		for(int index = 0; index < num; ++index) 
		{
			EventHandler* handlerp = (EventHandler*)events[index].data.ptr;
			handlerp->EnqueEvents(events[index].events);
		}
		
	}	
	
	return 0;
}
