#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>

#include "errno.h"

#include "EventThread.h"
#include "TcpSession.h"

TcpSession::TcpSession(int fd, struct sockaddr_in * addr)
{
	m_fd = fd;
	memcpy(&m_addr, addr, sizeof(struct sockaddr_in));
	fprintf(stdout, "%s: fd=%d, 0x%08X:%u\n", __PRETTY_FUNCTION__, m_fd, m_addr.sin_addr.s_addr, m_addr.sin_port);
}

TcpSession::~TcpSession()
{
}

int TcpSession::Init()
{
	//set options on the socket
    //we are a server, always disable nagle algorithm
    int one = 1;
    int err = ::setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(int));
    //AssertV(err == 0, OSThread::GetErrno());
    
    err = ::setsockopt(m_fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&one, sizeof(int));
    //AssertV(err == 0, OSThread::GetErrno());

    int sndBufSize = 96L * 1024L;
    err = ::setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, (char*)&sndBufSize, sizeof(int));
    //AssertV(err == 0, OSThread::GetErrno());

	// InitNonBlocking
   	int flag = ::fcntl(m_fd, F_GETFL, 0);
    err = ::fcntl(m_fd, F_SETFL, flag | O_NONBLOCK);
        
	g_event_thread->m_events.AddWatch(m_fd, EVENT_READ, this);
	
	return 0;
}

int TcpSession::Run()
{
	char buf[1024];
	size_t len = 1024;
	
	while(1)
	{
		ssize_t ret = recv(m_fd, buf, len, 0);
		if(ret > 0)
		{
			fprintf(stdout, "%s: recv %ld, %s\n", __PRETTY_FUNCTION__, ret, buf);
		}
		else if(ret == 0)
		{
			fprintf(stdout, "%s: recv %ld, close fd=%d\n", __PRETTY_FUNCTION__, ret, m_fd);
			close(m_fd);
			m_fd = -1;
			return -1;
		}
		else
		{
			int err = errno;
			fprintf(stderr, "%s: errno=%d, %s\n", __PRETTY_FUNCTION__, err, strerror(err));
			if(err == EAGAIN) // or other errno
			{
				return 0;
			}
			else
			{
				fprintf(stdout, "%s: recv %ld, close fd=%d\n", __PRETTY_FUNCTION__, ret, m_fd);
				close(m_fd);
				m_fd = -1;
				return -1;
			}
		}
	}
	
	return 0;
}
