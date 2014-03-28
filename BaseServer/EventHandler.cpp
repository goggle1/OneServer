#include <stdio.h>

#include "EventHandler.h"

EventHandler::EventHandler()
{	
	dequeh_init(&m_events_queue);
}

EventHandler::~EventHandler()
{
	// todo:
}

int EventHandler::EnqueEvents(u_int32_t events)
{	
	fprintf(stdout, "%s: events=0x%08X\n", __PRETTY_FUNCTION__, events);	
	
	// events into deque.
	dequeh_append(&m_events_queue, reinterpret_cast<void*>(events));
	// signal (choose one task thread, into task)
	Signal();
	
	return 0;
}

int EventHandler::Run()
{	
	while(1)
	{
		void* elementp = dequeh_remove_head(&m_events_queue);
		if(elementp == NULL)
		{
			break;
		}

		u_int64_t temp = reinterpret_cast<u_int64_t>(elementp);
		u_int32_t events = temp;
		fprintf(stdout, "%s: events=0x%08X\n", __PRETTY_FUNCTION__, events);	
	}
	return 0;
}

