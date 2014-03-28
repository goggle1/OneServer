#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include <sys/types.h>
#include "dequeH.h"
#include "task.h"

class EventHandler : public Task
{
public:
	EventHandler();
	~EventHandler();
	int EnqueEvents(u_int32_t events);
	
	virtual int Run();
	
protected:
	DEQUEH_T 	m_events_queue;
	// lock
	// EventThread.
};

#endif
