#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <sys/types.h>

#define EVENT_READ		0x00000001
#define EVENT_WRITE		0x00000002
#define EVENT_CONTINUE	0x00000100
#define EVENT_TIMEOUT	0x00000200


#define EPOLL_SIZE 		1000000
#define MAX_EVENTS		10000
// ms
#define WAIT_PERIOD		1000


class Events
{
public:
	Events();
	~Events();
	
	int Init();
	int AddWatch(int fd, u_int32_t events, void* handler);
	int ModifyWatch(int fd, u_int32_t events, void* handler);
	int DeleteWatch(int fd);
	
	int	m_epoll_fd;
	//int m_fd_num;	// now, we don't care. maybe care about it later.
	
protected:

};



#endif
