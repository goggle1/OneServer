#ifndef __TCPSESSION_H__
#define __TCPSESSION_H__

#include <netinet/in.h>
#include "EventHandler.h"

class TcpSession : public EventHandler
{
public:
	TcpSession(int fd, struct sockaddr_in* addr);
	~TcpSession();
	int Init();
	virtual int Run();
protected:
	int m_fd;
	struct sockaddr_in m_addr;
};

#endif