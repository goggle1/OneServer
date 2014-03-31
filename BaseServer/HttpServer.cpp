#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "HttpServer.h"

HttpServer::HttpServer()
{
}

HttpServer::~HttpServer()
{
}

int HttpServer::Run()
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

HttpSession* HttpServer::GetSession(int fd, struct sockaddr_in * addr)
{
	HttpSession* sessionp = new HttpSession(fd, addr);
	sessionp->Init();
	return sessionp;
}
