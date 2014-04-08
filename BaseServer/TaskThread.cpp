#include <unistd.h>
#include <stdio.h>
#include <sys/prctl.h>

#include "OS.h"
#include "events.h"
#include "EventThread.h"
#include "TaskThread.h"

TaskThread::TaskThread()
{
	dequeh_init(&m_task_queue);
}

TaskThread::~TaskThread()
{
	
}

int TaskThread::EnqueTask(Task * taskp)
{
	int ret = 0;
	{
        OSMutexLocker theLocker(&fMutex);
        ret = dequeh_append(&m_task_queue, taskp);
        if(ret < 0)
        {
    		return ret;
    	}
    }    
    	
    fCond.Signal();
	return ret;
}

Task* TaskThread::WaitForTask()
{
	while(1)
	{	
		int64_t theCurrentTime = OS::Milliseconds();
		
		OSHeapElem* elemp = fHeap.PeekMin();
        if (elemp != NULL)
        {	 
        	int64_t theTaskTime = elemp->GetValue();
        	if(theTaskTime <= theCurrentTime)
        	{
        		elemp = fHeap.ExtractMin();
        		Task* taskp = (Task*)elemp->GetEnclosingObject();     
        		taskp->EnqueEvents(EVENT_CONTINUE);
        		return taskp;
            }
        }

        //if there is an element waiting for a timeout, figure out how long we should wait.
        int64_t theTimeout = 0;
        if (elemp != NULL)
        { 
        	int64_t theTaskTime = elemp->GetValue();
            theTimeout = theTaskTime - theCurrentTime;
        }
        
        //
        // Make sure we can't go to sleep for some ridiculously short
        // period of time
        // Do not allow a timeout below 10 ms without first verifying reliable udp 1-2mbit live streams. 
        // Test with streamingserver.xml pref reliablUDP printfs enabled and look for packet loss and check client for  buffer ahead recovery.
	    if (theTimeout < 10) 
	    {
           theTimeout = 10;
        }
            
        //wait...        
        OSMutexLocker theLocker(&fMutex);
        if (m_task_queue.count == 0) 
        {
        	fCond.Wait(&fMutex, theTimeout);
        }    
        
		Task* taskp = (Task*)dequeh_remove_head(&m_task_queue);
		return taskp;
	}

	return NULL;
}

int TaskThread::Entry()
{
	prctl(PR_SET_NAME, "oneserver_work");
	
	Task* taskp = NULL;
    
    while(1) 
    {
        taskp = this->WaitForTask();
        if (taskp == NULL)
        {
        	continue;
        }
        		
		int task_ret = taskp->Run();			
		if(task_ret == -1)
		{			
			taskp->SetValid(false);
			g_event_thread->m_GrimReaper.EnqueDeath(taskp);
		}
		else if(task_ret == 0)
		{
			taskp->Detach();
			//this->EnqueTask(taskp);
		}
		else
		{
			taskp->fTimerHeapElem.SetValue(OS::Milliseconds() + task_ret);
            fHeap.Insert(&taskp->fTimerHeapElem);			
		}	
		
	}
	
	return 0;
}

