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
		int num = epoll_wait(m_EventsMaster.m_epoll_fd, events, MAX_EVENTS, WAIT_PERIOD);
		for(int index = 0; index < num; ++index) 
		{			 
			Task* taskp = (Task*)events[index].data.ptr;	
			if(events[index].events & EPOLLERR)
			{
				m_EventsMaster.DeleteWatch(taskp->GetFd());
				delete taskp;
				continue;
			}
		
			if(taskp == &m_BirthAngel)
			{			
				m_BirthAngel.Run();
			}
			else
			{				
				if(taskp->IsServer())
				{
					// do nothing.
					//m_EventsMaster.ModifyWatch(taskp->GetFd(), 0, taskp);
				}
				else
				{
					m_EventsMaster.ModifyWatch(taskp->GetFd(), 0, taskp);	
				}
				taskp->EnqueEvents(events[index].events);
			}
		}			
		
	}	
	
	return 0;
}

