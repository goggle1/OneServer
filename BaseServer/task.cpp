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

int Task::Attach()
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
	OSMutexLocker theLocker(&fMutex);
    dequeh_append(&m_EventsQueue, reinterpret_cast<void*>(events));    
	this->Attach();
	
	return 0;
}

int Task::DequeEvents(u_int32_t& events)
{
	OSMutexLocker theLocker(&fMutex);
	void* elementp = dequeh_remove_head(&m_EventsQueue);
	if(elementp == NULL)
	{
		this->Detach();
		return 0;
	}
	
	u_int64_t temp = reinterpret_cast<u_int64_t>(elementp);
	events = temp;	
	fprintf(stdout, "%s: events=0x%08X\n", __PRETTY_FUNCTION__, events);
	return 1;		
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


