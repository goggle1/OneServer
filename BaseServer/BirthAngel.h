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
	int			GiveBirth();
	virtual int	Run();

	int 		m_Pipes[2];	
	DEQUEH_T	m_BirthDeque;
	OSMutex     m_Mutex;
};

#endif
