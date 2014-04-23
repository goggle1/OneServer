#include <stdio.h>

#include "task.h"
#include "EventThread.h"
#include "TaskThread.h"
#include "TaskThreadPool.h"

Task::Task(bool is_server) : 
	fTimerHeapElem(this),
	fIdleElem(this)
{	
	m_is_server = is_server;
	m_IsValid = true;
	m_task_thread = NULL;
	m_events = 0;
	m_event_mask = 0;
	dequeh_init(&m_EventsQueue);
	//fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
}

Task::~Task()
{
	dequeh_release(&m_EventsQueue, NULL);
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
	OSMutexLocker theLocker(&fMutex);
	m_task_thread = NULL;
}

int Task::EnqueEvents(u_int64_t events)
{	
	fprintf(stdout, "%s[%p]: events=0x%lx\n", __PRETTY_FUNCTION__, this, events);
	u_int64_t old_events = __sync_fetch_and_or (&m_events, events);
	this->Attach();
	return 0;
	
	OSMutexLocker theLocker(&fMutex);
	if(m_IsValid)
	{
	    dequeh_append(&m_EventsQueue, (void*)events);    
		this->Attach();
		return 0;
	}
	
	return -1;
}

int Task::DequeEvents(u_int64_t& events)
{	
	events = __sync_fetch_and_and (&m_events, 0);
	fprintf(stdout, "%s[%p]: events=0x%lx\n", __PRETTY_FUNCTION__, this, events);
	return 0;
	
	OSMutexLocker theLocker(&fMutex);	
	void* elementp = dequeh_remove_head(&m_EventsQueue);
	if(elementp == NULL)
	{
		fprintf(stdout, "%s[%p]: events=[null], thread=%p\n", __PRETTY_FUNCTION__, this, m_task_thread);
		return 0;
	}
	
	events = (u_int64_t)elementp;
	fprintf(stdout, "%s[%p]: events=0x%lx\n", __PRETTY_FUNCTION__, this, events);
	return 1;		
}

int Task::RequestEvent(u_int64_t event)
{
	int ret = 0;
	m_event_mask = event;
	g_event_thread->m_BirthAngel.EnqueChild(this);
	return ret;
}

void Task::Release()
{
	fprintf(stdout, "%s[%p]: \n", __PRETTY_FUNCTION__, this);
	// do nothing.
}

int Task::SetInvalid()
{
	fprintf(stdout, "%s[%p]: \n", __PRETTY_FUNCTION__, this);
	//OSMutexLocker theLocker(&fMutex);
	if(m_IsValid)
	{
		m_IsValid = false;
		Release();
		return 0;
	}
	
	return -1;
}


