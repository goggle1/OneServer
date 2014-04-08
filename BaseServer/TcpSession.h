#ifndef __TCPSESSION_H__
#define __TCPSESSION_H__

#include <netinet/in.h>

#include "StrPtrLen.h"
#include "task.h"

#define REQUEST_BUFF_SIZE	(1024*16)
#define RESPONSE_BUFF_SIZE	(1024*16)

class TcpSession : public Task
{
public:
	TcpSession(int fd, struct sockaddr_in* addr);
	virtual ~TcpSession();
	int 			Init();
	int				DoRead();
	virtual int 	Run();	
	
protected:
	int 				m_fd;
	struct sockaddr_in 	m_addr;
	
	char				m_RequestBuffer[REQUEST_BUFF_SIZE];
	StrPtrLen			m_StrReceived;
	StrPtrLen			m_StrRequest;

	char				m_ResponseBuffer[RESPONSE_BUFF_SIZE];			
	StrPtrLen			m_StrResponse; 
	StrPtrLen			m_StrRemained;
	
};

#endif