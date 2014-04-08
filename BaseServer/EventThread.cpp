#include <sys/prctl.h>
#include <sys/epoll.h>

#include "task.h"
#include "EventThread.h"

EventThread::EventThread()
{	
}

EventThread::~EventThread()
{
}

int EventThread::Init()
{
	m_EventsMaster.Init();
	m_GrimReaper.Init();
	return 0;
}

int EventThread::Entry()
{
	prctl(PR_SET_NAME, "oneserver_events");
	
	struct epoll_event events[MAX_EVENTS];
	while(1)
	{	
		int invalid_num = 0;
		int num = epoll_wait(m_EventsMaster.m_epoll_fd, events, MAX_EVENTS, WAIT_PERIOD);
		for(int index = 0; index < num; ++index) 
		{
			Task* taskp = (Task*)events[index].data.ptr;
			if(taskp!=&m_GrimReaper && taskp->IsValid())
			{
				taskp->EnqueEvents(events[index].events);
			}
			else
			{
				invalid_num ++;
			}
		}
		if(invalid_num > 0)
		{
			m_GrimReaper.EnqueEvents(EVENT_READ);
		}		
		
	}	
	
	return 0;
}

