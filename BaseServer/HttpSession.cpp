#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "events.h"
#include "StringParser.h"
#include "HttpSession.h"

HttpSession::HttpSession(int fd, struct sockaddr_in * addr) : 
	TcpSession(fd, addr)
{
	fprintf(stdout, "%s: fd=%d, 0x%08X:%u\n", __PRETTY_FUNCTION__, m_fd, m_addr.sin_addr.s_addr, m_addr.sin_port);
}

HttpSession::~HttpSession()
{
	fprintf(stdout, "%s: fd=%d, 0x%08X:%u\n", __PRETTY_FUNCTION__, m_fd, m_addr.sin_addr.s_addr, m_addr.sin_port);
}

bool HttpSession::IsFullRequest()
{
	m_StrRequest.Ptr = m_StrReceived.Ptr;
    m_StrRequest.Len = 0;
    
    StringParser headerParser(&m_StrReceived);
    while (headerParser.GetThruEOL(NULL))
    {        
        if (headerParser.ExpectEOL())
        {
            m_StrRequest.Len = headerParser.GetDataParsedLen();
            return true;
        }
    }
    
    return false;
}

int HttpSession::DoGet()
{
	int ret = 0;
	fprintf(stdout, "%s: %s\n", __PRETTY_FUNCTION__, m_Request.fRequestPath);
	// todo:
	
	return 0;
}

int HttpSession::DoRequest()
{
	int ret = 0;
	
	m_Request.Clear();
	m_Request.Parse(&m_StrRequest);
	if(m_Request.fMethod == httpGetMethod)
	{
		ret = DoGet();
	}
	else if(m_Request.fMethod == httpPostMethod)
	{
		//ret = DoPost();
	}
	else if(m_Request.fMethod == httpHeadMethod)
	{
		//ret = DoHead();
	}
	
	return ret;
}

int HttpSession::DoRead()
{
	int ret = 0;
	
	while(1)
	{
		char* 	recv_bufferp	= m_RequestBuffer + m_StrReceived.Len;
		ssize_t recv_buff_size	= REQUEST_BUFF_SIZE - m_StrReceived.Len;
		if(recv_buff_size <= 0)
		{
			fprintf(stderr, "%s: recv buffer full, recv_buff_size=%ld\n", __PRETTY_FUNCTION__, recv_buff_size);
			close(m_fd);
			m_fd = -1;
			return -1;
		}
		
		ssize_t recv_ret = recv(m_fd, recv_bufferp, recv_buff_size, 0);
		if(recv_ret == 0)
		{
			fprintf(stdout, "%s: recv=%ld, close fd=%d\n", __PRETTY_FUNCTION__, recv_ret, m_fd);
			close(m_fd);
			m_fd = -1;
			return -1;
		}
		else if(recv_ret < 0)
		{
			int err = errno;
			fprintf(stderr, "%s: errno=%d, %s\n", __PRETTY_FUNCTION__, err, strerror(err));
			if(err == EAGAIN) // or other errno
			{
				return 0;
			}
			else
			{
				fprintf(stderr, "%s: recv=%ld, close fd=%d\n", __PRETTY_FUNCTION__, recv_ret, m_fd);
				close(m_fd);
				m_fd = -1;
				return -1;
			}
		}
		fprintf(stdout, "%s: size=%ld, recv=%ld, %s\n", __PRETTY_FUNCTION__, recv_buff_size, recv_ret, recv_bufferp);
		m_StrReceived.Len += recv_ret;

		if(IsFullRequest())
		{
			ret = DoRequest();
			if(ret < 0)
			{
				return ret;
			}
		}				
	}

	return 0;
	
}

int HttpSession::Run()
{	
	int ret = 0;
	
	while(1)
	{
		void* elementp = dequeh_remove_head(&m_events_queue);
		if(elementp == NULL)
		{
			break;
		}

		u_int64_t temp = reinterpret_cast<u_int64_t>(elementp);
		u_int32_t events = temp;
		fprintf(stdout, "%s: events=0x%08X\n", __PRETTY_FUNCTION__, events);
		if(events | EVENT_READ)
		{
			ret = DoRead();			
		}
		
		if(ret < 0)
		{
			return ret;
		}
	}
	
	return 0;
}

