#ifndef __EVENTTHREAD_H__
#define __EVENTTHREAD_H__

#include "events.h"
#include "thread.h"
#include "GrimReaper.h"

class EventThread : public Thread
{
public:
	EventThread();
    ~EventThread();    
    int 	Init();
    virtual int Entry();

	Events		m_EventsMaster;
	GrimReaper	m_GrimReaper;
};

extern EventThread* 	g_event_thread;

#endif
