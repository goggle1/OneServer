#ifndef __TASK_THREAD_H__
#define __TASK_THREAD_H__

#include "dequeH.h"
#include "task.h"
#include "thread.h"

class TaskThread : public Thread
{
public:
	TaskThread();
	~TaskThread();
	int		EnqueTask(Task* taskp);
	int		Signal();
	Task*	WaitForTask();
	
	virtual int Entry();

protected:
	DEQUEH_T	m_task_queue;
	// lock

	OSHeap              fHeap;	
};

#endif
