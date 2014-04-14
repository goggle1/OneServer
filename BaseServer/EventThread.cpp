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
	m_BirthAngel.Init();
	m_GrimReaper.Init();
	return 0;
}

int EventThread::Entry()
{
	prctl(PR_SET_NAME, "oneserver_events");
	
	struct epoll_event events[MAX_EVENTS];
	while(1)
	{	
		bool BirthAngel_is_called = false;
		bool GrimReaper_is_called = false;
		int num = epoll_wait(m_EventsMaster.m_epoll_fd, events, MAX_EVENTS, WAIT_PERIOD);
		for(int index = 0; index < num; ++index) 
		{
			if( events[index].events & EPOLLIN)
			{			 
				Task* taskp = (Task*)events[index].data.ptr;
				if(taskp == &m_BirthAngel)
				{
					BirthAngel_is_called = true;
				}
				else if(taskp == &m_GrimReaper)
				{
					GrimReaper_is_called = true;
				}
				else
				{
					int ret = taskp->EnqueEvents(events[index].events);
					if(ret < 0)
					{
						m_EventsMaster.DeleteWatch(taskp->GetFd());
					}
				}
			}
		}
		if(BirthAngel_is_called)
		{
			m_BirthAngel.GiveBirth();
		}
		if(GrimReaper_is_called)
		{
			m_GrimReaper.EnqueEvents(EVENT_READ);
		}		
		
	}	
	
	return 0;
}

