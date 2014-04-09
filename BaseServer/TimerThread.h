#ifndef __TIMERTHREAD_H__
#define __TIMERTHREAD_H__

#include "heap.h"
#include "OSMutex.h"
#include "OSCond.h"
#include "thread.h"

class TimerThread : public Thread
{
public:
	TimerThread();
	virtual ~TimerThread();
	int				SetTimeout(Task* taskp, int64_t msec);
    int 			CancelTimeout(Task *idleObj);
	virtual int 	Entry();

protected:
	OSHeap  	fIdleHeap;
    OSMutex 	fHeapMutex;
    OSCond  	fHeapCond;
};

extern TimerThread*	g_timer_thread;

#endif
