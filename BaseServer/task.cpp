#include <stdio.h>

#include "task.h"
#include "TaskThread.h"
#include "TaskThreadPool.h"

Task::Task() : 
	fTimerHeapElem(this)
{
	m_task_thread = NULL;
	dequeh_init(&m_events_queue);
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
		m_task_thread->Signal();
	}
	else
	{
		m_task_thread->Signal();
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
	
	// events into deque.
	dequeh_append(&m_events_queue, reinterpret_cast<void*>(events));
	// signal (choose one task thread, into task)
	Signal();
	
	return 0;
}


int Task::Run()
{
	// do nothing, just one example.
	fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
	
	return 0;
}

