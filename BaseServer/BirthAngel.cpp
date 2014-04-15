
#include <stdio.h>
#include <unistd.h>

#include "EventThread.h"
#include "BirthAngel.h"

BirthAngel::BirthAngel()
{
	fprintf(stdout, "%s[%p]: \n", __PRETTY_FUNCTION__, this);
	dequeh_init(&m_BirthDeque);	
}

BirthAngel::~BirthAngel()
{
	fprintf(stdout, "%s[%p]: \n", __PRETTY_FUNCTION__, this);
	dequeh_release(&m_BirthDeque, task_release);	
}

int BirthAngel::Init()
{
	::pipe(m_Pipes);	
	g_event_thread->m_EventsMaster.AddWatch(m_Pipes[0], EVENT_READ, this);	
	return 0;
}

int BirthAngel::EnqueBirth(Task * taskp)
{
	fprintf(stdout, "%s[%p]: taskp=%p\n", __PRETTY_FUNCTION__, this, taskp);
	{
		OSMutexLocker theLocker(&m_Mutex);
		dequeh_append(&m_BirthDeque, taskp);
	}
	write(m_Pipes[1], (void*)"B", 1);
	return 0;
}

int BirthAngel::GiveBirth()
{
	fprintf(stdout, "%s[%p]: \n", __PRETTY_FUNCTION__, this);
	
	OSMutexLocker theLocker(&m_Mutex);
	while(1)
	{	
		void* elementp = NULL;
		{
			//OSMutexLocker theLocker(&m_Mutex);
			elementp  = dequeh_remove_head(&m_BirthDeque);
			if(elementp == NULL)
			{
				return 0;
			}
		}

		char temp[4];
		read(m_Pipes[0], temp, 1);
		
		Task* taskp = (Task*)elementp;
		fprintf(stdout, "%s[%p]: taskp=%p\n", __PRETTY_FUNCTION__, this, taskp);
		g_event_thread->m_EventsMaster.AddWatch(taskp->GetFd(), EVENT_READ, taskp);			
	}

	return 0;
}

int BirthAngel::Run()
{
	// it should never be called.
	return 0;
}


