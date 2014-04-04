#include <unistd.h>
#include <sys/prctl.h>

#include "OS.h"
#include "events.h"
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
	ret = dequeh_append(&m_task_queue, taskp);
	return ret;
}

int TaskThread::Signal()
{
	// todo:
	return 0;
}

Task* TaskThread::WaitForTask()
{
	while(1)
	{	
		OSHeapElem* elemp = fHeap.PeekMin();
        if (elemp != NULL)
        {    
        	int64_t theCurrentTime = OS::Milliseconds(); 
        	int64_t theTaskTime = elemp->GetValue();
        	if(theTaskTime <= theCurrentTime)
        	{
        		elemp = fHeap.ExtractMin();
        		Task* taskp = (Task*)elemp->GetEnclosingObject();     
        		taskp->EnqueEvents(EVENT_CONTINUE);
        		return taskp;
            }
        }
        
		void* elementp = dequeh_remove_head(&m_task_queue);
		if(elementp == NULL)
		{
			usleep(10);
			continue;
		}
		Task* taskp = (Task*)elementp;
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
        	return -1;
        }
        		
		int task_ret = taskp->Run();			
		if(task_ret == -1)
		{
			delete taskp;
		}
		else if(task_ret == 0)
		{
			taskp->Detach();
		}
		else
		{
			taskp->fTimerHeapElem.SetValue(OS::Milliseconds() + task_ret);
            fHeap.Insert(&taskp->fTimerHeapElem);			
		}	
		
	}
	
	return 0;
}

