#ifndef __TASK_H__
#define __TASK_H__

class TaskThread;

class Task
{
public:
	Task();
	virtual ~Task();
	int	Signal();
	void Detach();
	virtual int Run();
	
protected:
	TaskThread*	m_task_thread;
};

#endif