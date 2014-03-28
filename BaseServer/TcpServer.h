#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "TcpSession.h"
#include "EventHandler.h"

class TcpServer : public EventHandler
{
public:
	TcpServer();
	~TcpServer();
	virtual int Run();

	int 	Init(u_int32_t ip, u_int16_t port);
	int		GetFd()	{ return m_fd; };

	virtual TcpSession*   GetSession(int fd, struct sockaddr_in* addr);

protected:
	int		m_fd;
};

#endif
