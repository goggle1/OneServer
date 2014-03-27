#include <sys/prctl.h>
#include <unistd.h>
#include <sys/epoll.h>

#include "EventHandler.h"
#include "events.h"

#define EPOLL_SIZE 	1000000
#define MAX_EVENTS	10000
// ms
#define WAIT_PERIOD	1000

Events::Events()
{
	m_epoll_fd = -1;	
}

Events::~Events()
{
}

int Events::Init()
{
	m_epoll_fd = epoll_create(EPOLL_SIZE);
	if(m_epoll_fd < 0)
	{
		return -1;
	}
	
	return 0;
}

int Events::AddWatch(int fd, u_int32_t events, void* handler)
{
	int ret = 0;
	
	struct epoll_event epoll_event = {0};	
	epoll_event.events |= EPOLLET;	
	if(events&EVENT_READ)
	{
		epoll_event.events |= EPOLLIN;
	}
	if(events&EVENT_WRITE)
	{
		epoll_event.events |= EPOLLOUT;
	}

	epoll_event.data.ptr = handler;
	
	ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &epoll_event);
	
	return ret;
}

void* Events::Run(void* argp)
{
	prctl(PR_SET_NAME, "oneserver_events");

	Events* eventsp = (Events*)argp;

	struct epoll_event events[MAX_EVENTS];
	while(1)
	{		
		int num = epoll_wait(eventsp->m_epoll_fd, events, MAX_EVENTS, WAIT_PERIOD);
		for(int index = 0; index < num; ++index) 
		{
			EventHandler* handlerp = (EventHandler*)events[index].data.ptr;
			handlerp->EnqueEvents(events[index].events);
		}
	}	
	
	return NULL;
}


