#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "task.h"
#include "TcpSession.h"

class TcpServer : public Task
{
public:
	TcpServer();
	virtual ~TcpServer();
	int 			Init(u_int32_t ip, u_int16_t port);
	TcpSession*   	GetSession(int fd, struct sockaddr_in* addr);
	int				DoRead();
	virtual int 	Run();

protected:
	int		m_fd;
};

#endif
