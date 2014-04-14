
#include <sys/prctl.h>

#include "OS.h"
#include "events.h"
#include "task.h"
#include "TimerThread.h"

TimerThread::TimerThread()
{
}

TimerThread::~TimerThread()
{
}

int TimerThread::SetTimeout(Task* taskp,int64_t msec)
{
	int ret = 0;

	//note: OSHeap doesn't support a random remove, so this function
    //won't change the timeout value if there is already one set
    if (taskp->fIdleElem.IsMemberOfAnyHeap())
    {
        return -1;
    }
    taskp->fIdleElem.SetValue(OS::Milliseconds() + msec);
    
    {
        OSMutexLocker locker(&fHeapMutex);
        fIdleHeap.Insert(&taskp->fIdleElem);
    }
    fHeapCond.Signal();
    
	return ret;
}


int TimerThread::CancelTimeout(Task* taskp)
{
	int ret = 0;

	OSMutexLocker locker(&fHeapMutex);
    fIdleHeap.Remove(&taskp->fIdleElem);  
    
	return ret;
}

int TimerThread::Entry()
{
	prctl(PR_SET_NAME, "oneserver_timer");
	
	int ret = 0;

	OSMutexLocker locker(&fHeapMutex);	
	while(1)
	{
		//if there are no events to process, block.
        if (fIdleHeap.CurrentHeapSize() == 0)
        {
            fHeapCond.Wait(&fHeapMutex);
        }
        
        int64_t msec = OS::Milliseconds();        
        //pop elements out of the heap as long as their timeout time has arrived
        while ((fIdleHeap.CurrentHeapSize() > 0) && (fIdleHeap.PeekMin()->GetValue() <= msec))
        {
            Task* taskp = (Task*)fIdleHeap.ExtractMin()->GetEnclosingObject(); 
           	taskp->EnqueEvents(EVENT_TIMEOUT);
        }
                        
        //we are done sending idle events. If there is a lowest tick count, then
        //we need to sleep until that time.
        if (fIdleHeap.CurrentHeapSize() > 0)
        {
            int64_t timeoutTime = fIdleHeap.PeekMin()->GetValue();
            //because sleep takes a 32 bit number
            timeoutTime -= msec;
            u_int32_t smallTime = (u_int32_t)timeoutTime;
            fHeapCond.Wait(&fHeapMutex, smallTime);
        }
	}

	return ret;
}
