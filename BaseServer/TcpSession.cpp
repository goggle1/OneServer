#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "common.h"
#include "EventThread.h"
#include "TcpSession.h"

TcpSession::TcpSession(int fd, struct sockaddr_in * addr)
{
	m_fd = fd;	
	memcpy(&m_addr, addr, sizeof(struct sockaddr_in));
	m_StrReceived.Ptr = m_RequestBuffer;
	m_StrReceived.Len = 0;
	fprintf(stdout, "%s: fd=%d, 0x%08X:%u\n", __PRETTY_FUNCTION__, m_fd, m_addr.sin_addr.s_addr, m_addr.sin_port);
}

TcpSession::~TcpSession()
{	
	fprintf(stdout, "%s: fd=%d, 0x%08X:%u\n", __PRETTY_FUNCTION__, m_fd, m_addr.sin_addr.s_addr, m_addr.sin_port);
	close(m_fd);
	m_fd = -1;
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
        
	g_event_thread->m_EventsMaster.AddWatch(m_fd, EVENT_READ, this);
	
	return 0;
}

int TcpSession::DoRead()
{
	char r_buffer[1024];
	size_t len = 1024;

	char s_buffer[1024];
	snprintf(s_buffer, 1024, "%s: I got it.\n", __PRETTY_FUNCTION__);
	size_t s_len = strlen(s_buffer);
	
	while(1)
	{
		ssize_t r_ret = recv(m_fd, r_buffer, len, 0);
		if(r_ret == 0)
		{
			fprintf(stdout, "%s: recv %ld, from fd=%d\n", __PRETTY_FUNCTION__, r_ret, m_fd);
			return -1;
		}
		else if(r_ret < 0)
		{
			int err = errno;
			fprintf(stderr, "%s: errno=%d, %s\n", __PRETTY_FUNCTION__, err, strerror(err));
			if(err == EAGAIN) // or other errno
			{
				return 0;
			}
			else
			{
				fprintf(stdout, "%s: recv %ld, close fd=%d\n", __PRETTY_FUNCTION__, r_ret, m_fd);
				return -1;
			}
		}
		r_buffer[r_ret] = '\0';
		fprintf(stdout, "%s: recv %ld,\n%s\n", __PRETTY_FUNCTION__, r_ret, r_buffer);
		
		ssize_t s_ret = send(m_fd, s_buffer, s_len, 0);	
		if(s_ret == -1)
		{
			int err = errno;
			fprintf(stderr, "%s: errno=%d, %s\n", __PRETTY_FUNCTION__, err, strerror(err));
			if(err == EAGAIN) // or other errno
			{
				return 0;
			}
			else
			{
				fprintf(stdout, "%s: send %ld, from fd=%d\n", __PRETTY_FUNCTION__, s_ret, m_fd);
				return -1;
			}
		}
		fprintf(stdout, "%s: send %ld, return %ld, %s\n", __PRETTY_FUNCTION__, s_len, s_ret, s_buffer);
		
	}
	
	return 0;
}

int TcpSession::Run()
{	
	int ret = 0;
	
	while(1)
	{		
		void* elementp = NULL;
		{
			OSMutexLocker theLocker(&fMutex);
			elementp = dequeh_remove_head(&m_EventsQueue);
			if(elementp == NULL)
			{
				break;
			}
		}

		u_int64_t temp = reinterpret_cast<u_int64_t>(elementp);
		u_int32_t events = temp;
		fprintf(stdout, "%s: events=0x%08X\n", __PRETTY_FUNCTION__, events);
		if(events & EVENT_READ)
		{
			ret = DoRead();			
		}
		else if(events & EVENT_CONTINUE)
		{
			//ret = DoContinue();			
		}
		else if(events & EVENT_TIMEOUT)
		{
			//ret = DoTimeout();			
		}
		
		if(ret < 0)
		{
			return ret;
		}
	}
	
	return ret;
}


