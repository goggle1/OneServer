#ifndef __HTTPSESSION_H__
#define __HTTPSESSION_H__

#include "HttpRequest.h"
#include "TcpSession.h"

class HttpSession : public TcpSession
{
public:
	HttpSession(int fd, struct sockaddr_in* addr);
	virtual ~HttpSession();

	virtual int 	Run();
	virtual int 	DoRead();
	virtual bool 	IsFullRequest();
	virtual int		DoRequest();	
	
protected:
	int			DoGet();
	
	HttpRequest 	m_Request;

};

#endif