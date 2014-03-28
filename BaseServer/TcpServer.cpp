#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
       
#include "TcpServer.h"

TcpServer::TcpServer()
{
	m_fd = -1;
}

TcpServer::~TcpServer()
{
}

int TcpServer::Run()
{
	int ret = 0;

	fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);

	while(1)
	{
		struct sockaddr_in addr;
		socklen_t size = sizeof(addr);
		int a_fd = accept(m_fd, (struct sockaddr*)&addr, &size);
		if (a_fd == -1)
		{
	        int acceptError = errno;
	        fprintf(stderr, "%s: errno=%d, %s\n", __PRETTY_FUNCTION__, acceptError, strerror(acceptError));
	        if (acceptError == EAGAIN)
	        {   
	            return 0;
	        }		        
			else if (acceptError == EMFILE || acceptError == ENFILE)
	        {	
				exit (-1);	
	        }
	        else
	        {
	        	// do nothing.
	        }
	   	}
	    else
	    {
	    	TcpSession* sessionp = this->GetSession(a_fd, &addr);
		    if (sessionp == NULL)
		    { 
		        close(a_fd);		        
		    }		    
	    }
    }
	
	return ret;
}

int TcpServer::Init(u_int32_t ip, u_int16_t port)
{
	int ret = 0;

	m_fd = ::socket(PF_INET, SOCK_STREAM, 0);
	if(m_fd < 0)
	{
		return -1;
	}

	int flag = ::fcntl(m_fd, F_GETFL, 0);
    ret = ::fcntl(m_fd, F_SETFL, flag | O_NONBLOCK);
    if(m_fd < 0)
	{
		return -2;
	}
    
    int one = 1;
    ret = ::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(int));
    if(m_fd < 0)
	{
		return -3;
	}

	struct sockaddr_in  fLocalAddr;
    ::memset(&fLocalAddr, 0, sizeof(fLocalAddr));
    fLocalAddr.sin_family = AF_INET;
    fLocalAddr.sin_port = htons(port);
    fLocalAddr.sin_addr.s_addr = htonl(ip);    
    ret = ::bind(m_fd, (sockaddr *)&fLocalAddr, sizeof(fLocalAddr));
    

    int bufSize = 96*1024;
    ret = ::setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, (char*)&bufSize, sizeof(int));
    if(m_fd < 0)
	{
		return -4;
	}

    ret = ::listen(m_fd, 100);
    if(m_fd < 0)
	{
		return -5;
	}

	
	return ret;
}

TcpSession* TcpServer::GetSession(int fd, struct sockaddr_in * addr)
{
	TcpSession* sessionp = new TcpSession(fd, addr);
	sessionp->Init();
	return sessionp;
}

