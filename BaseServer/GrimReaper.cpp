#include <unistd.h>
#include <stdio.h>

#include "EventThread.h"
#include "GrimReaper.h"


GrimReaper::GrimReaper()
{
	fprintf(stdout, "%s[%p]: \n", __PRETTY_FUNCTION__, this);
	dequeh_init(&m_DeathDeque);	
}

GrimReaper::~GrimReaper()
{
	fprintf(stdout, "%s[%p]: \n", __PRETTY_FUNCTION__, this);
	dequeh_release(&m_DeathDeque, task_release);	
}

int GrimReaper::Init()
{
	::pipe(m_Pipes);	
	g_event_thread->m_EventsMaster.AddWatch(m_Pipes[0], EVENT_READ, this);	
	return 0;
}

int GrimReaper::EnqueDeath(Task * taskp)
{
	fprintf(stdout, "%s[%p]: taskp=%p\n", __PRETTY_FUNCTION__, this, taskp);
	{
		OSMutexLocker theLocker(&m_Mutex);
		dequeh_append(&m_DeathDeque, taskp);
	}
	write(m_Pipes[1], (void*)"D", 1);
	return 0;
}

int GrimReaper::DoRead()
{
	while(1)
	{	
		void* elementp = NULL;
		{
			OSMutexLocker theLocker(&m_Mutex);
			elementp  = dequeh_remove_head(&m_DeathDeque);
			if(elementp == NULL)
			{
				return 0;
			}
		}

		char temp[4];
		read(m_Pipes[0], temp, 1);
		
		Task* taskp = (Task*)elementp;
		fprintf(stdout, "%s[%p]: taskp=%p\n", __PRETTY_FUNCTION__, this, taskp);
		delete taskp;		
	}

	return 0;
}

int GrimReaper::Run()
{
	int ret = 0;		
	while(1)
	{		
		u_int32_t events = 0;
		ret = DequeEvents(events);
		if(ret < 1)
		{
			return 0;
		}
		
		fprintf(stdout, "%s[%p]: events=0x%08X\n", __PRETTY_FUNCTION__, this, events);
		if(events & EVENT_READ)
		{
			ret = DoRead(); 		
		}
	}

	return 0;
}

