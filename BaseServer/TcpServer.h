#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "TcpSession.h"
#include "EventHandler.h"

class TcpServer : public EventHandler
{
public:
	TcpServer();
	virtual ~TcpServer();
	int 	Init(u_int32_t ip, u_int16_t port);
	int		GetFd()	{ return m_fd; };
	TcpSession*   GetSession(int fd, struct sockaddr_in* addr);
	
	virtual int Run();	

protected:
	int		m_fd;
};

#endif
