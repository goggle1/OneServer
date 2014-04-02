#include <unistd.h>
#include <sys/prctl.h>

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

int TaskThread::Entry()
{
	prctl(PR_SET_NAME, "oneserver_work");
	
	while(1)
	{
		void* elmentp = dequeh_remove_head(&m_task_queue);
		if(elmentp == NULL)
		{
			sleep(1);
		}
		else
		{
			Task* taskp = (Task*)elmentp;
			taskp->Detach();
			int ret = taskp->Run();			
			if(ret == -1)
			{
				delete taskp;
			}
			else if(ret == 0)
			{
				// do nothing.
			}
			else
			{
				// add to timer heap.
				
			}
		}
	}
	
	return 0;
}
