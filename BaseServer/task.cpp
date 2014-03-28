#include <stdio.h>

#include "task.h"
#include "TaskThread.h"
#include "TaskThreadPool.h"

Task::Task()
{
	m_task_thread = NULL;
}

Task::~Task()
{
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

int Task::Run()
{
	// do nothing, just one example.
	fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
	
	return 0;
}

