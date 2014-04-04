#ifndef __TASK_H__
#define __TASK_H__

#include "heap.h"
#include "dequeH.h"

class TaskThread;

class Task
{
public:
	Task();
	virtual ~Task();
	int		Signal();
	void 	Detach();
	int 	EnqueEvents(u_int32_t events);
	
	virtual int Run();
	
protected:	
	TaskThread*	m_task_thread;

	// lock, todo:
	DEQUEH_T 	m_events_queue;	
	
	//This could later be optimized by using a timing wheel instead of a heap,
    //and that way we wouldn't need both a heap elem and a queue elem here (just queue elem)
    OSHeapElem      fTimerHeapElem;

    friend class TaskThread;
};

#endif