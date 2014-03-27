#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include <sys/types.h>

class EventHandler
{
public:
	EventHandler();
	~EventHandler();
	int GetFd()	{ return m_fd; };
	int EnqueEvents(u_int32_t events);
	
	virtual int Run();
	
protected:
	int 	m_fd;
	//DEQUE_T m_events_queue;
	// lock
	// EventThread.
};

#endif