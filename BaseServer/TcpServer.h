#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "EventHandler.h"

class TcpServer:public EventHandler
{
public:
	TcpServer();
	~TcpServer();
	virtual int Run();

	int 	Init(u_int32_t ip, u_int16_t port);
};

#endif
