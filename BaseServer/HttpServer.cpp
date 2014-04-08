#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "events.h"
#include "HttpServer.h"

HttpServer::HttpServer()
{
}

HttpServer::~HttpServer()
{
}

int HttpServer::DoRead()
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
			HttpSession* sessionp = this->GetSession(a_fd, &addr);
			if (sessionp == NULL)
			{ 
				close(a_fd);				
			}			
		}
	}
	
	return ret;
}


int HttpServer::Run()
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


HttpSession* HttpServer::GetSession(int fd, struct sockaddr_in * addr)
{
	HttpSession* sessionp = new HttpSession(fd, addr);
	sessionp->Init();
	return sessionp;
}
