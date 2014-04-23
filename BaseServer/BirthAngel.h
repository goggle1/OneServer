#ifndef __BIRTHANGEL_H__
#define __BIRTHANGEL_H__

#include "task.h"

class BirthAngel : public Task
{
public:
	BirthAngel();
	virtual ~BirthAngel();
	int 		Init();
	int 		EnqueBirth(Task* taskp);
	int 		EnqueChild(Task* taskp);
	int			GiveBirth();
	int			GiveChild();
	virtual int	Run();

	int 		m_Pipes[2];	
	DEQUEH_T	m_BirthDeque;
	OSMutex     m_BirthMutex;
	DEQUEH_T	m_ChildDeque;
	OSMutex     m_ChildMutex;
};

#endif
