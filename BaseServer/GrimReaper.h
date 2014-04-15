#ifndef __GRIPREAPER_H__
#define __GRIPREAPER_H__

#include "OSMutex.h"
#include "dequeH.h"
#include "task.h"

class GrimReaper : public Task
{
public:
	GrimReaper();
	virtual ~GrimReaper();
	int			Init();
	int			EnqueDeath(Task* taskp);
	int 		RemoveWatch();
	int			DoRead();
	virtual	int	Run();

protected:
	int 		m_Pipes[2];	
	OSMutex     m_Mutex;
	DEQUEH_T	m_DeathDeque;
	DEQUEH_T	m_DeathDeque2;
};

#endif