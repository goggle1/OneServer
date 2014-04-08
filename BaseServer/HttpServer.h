#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include "HttpSession.h"
#include "TcpServer.h"

class HttpServer : public TcpServer
{
public:
	HttpServer();
	virtual ~HttpServer();
	int		DoRead();
	virtual int Run();
	HttpSession*   GetSession(int fd, struct sockaddr_in* addr);

protected:
	
};

#endif