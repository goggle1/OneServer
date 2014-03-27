#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
       
#include "TcpServer.h"

TcpServer::TcpServer()
{
}

TcpServer::~TcpServer()
{
}

int TcpServer::Run()
{
	int ret = 0;
	
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
