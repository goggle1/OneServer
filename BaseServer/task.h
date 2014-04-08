#ifndef __TASK_H__
#define __TASK_H__

#include "OSCond.h"
#include "OSMutex.h"
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
	bool	IsValid();
	int		SetValid(bool value);
	virtual int Run() = 0;
	
protected:	
	TaskThread*	m_task_thread;
	
	DEQUEH_T 	m_EventsQueue;	
    OSMutex     fMutex;
	
	//This could later be optimized by using a timing wheel instead of a heap,
    //and that way we wouldn't need both a heap elem and a queue elem here (just queue elem)
    OSHeapElem      fTimerHeapElem;

    bool 		m_IsValid;

    friend class TaskThread;
};

#endif