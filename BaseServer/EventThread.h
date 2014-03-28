#ifndef __EVENTTHREAD_H__
#define __EVENTTHREAD_H__

#include "events.h"
#include "thread.h"

class EventThread : public Thread
{
public:
	EventThread();
    ~EventThread();    
    virtual int Entry();

	Events	m_events;
};

extern EventThread* 	g_event_thread;

#endif
