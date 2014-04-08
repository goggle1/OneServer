#include <stdio.h>

#include "task.h"
#include "TaskThread.h"
#include "TaskThreadPool.h"

Task::Task() : 
	fTimerHeapElem(this)
{
	m_IsValid = true;
	m_task_thread = NULL;
	dequeh_init(&m_EventsQueue);
	//fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
}

Task::~Task()
{
	//fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
}

int Task::Signal()
{
	// if no task thread, select one thread, then send signal to task thread.
	// otherwise, send signal to task thread.
	if(m_task_thread == NULL)
	{
		m_task_thread = g_task_thread_pool->SelectThread(THREAD_SHORT);
		m_task_thread->EnqueTask(this);
	}
	
	return 0;
}

void Task::Detach()
{
	m_task_thread = NULL;
}

int Task::EnqueEvents(u_int32_t events)
{	
	fprintf(stdout, "%s: events=0x%08X\n", __PRETTY_FUNCTION__, events);	

	{
        OSMutexLocker theLocker(&fMutex);
        dequeh_append(&m_EventsQueue, reinterpret_cast<void*>(events));
    }			
	Signal();
	
	return 0;
}


bool Task::IsValid()
{
	return m_IsValid;
}

int Task::SetValid(bool value)
{
	m_IsValid = value;
	return 0;
}


