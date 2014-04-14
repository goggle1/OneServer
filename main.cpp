#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <signal.h>

#include "BaseServer/EventThread.h"
#include "BaseServer/TaskThreadPool.h"
#include "BaseServer/TimerThread.h"
#include "BaseServer/FilesMaster.h"
#include "BaseServer/TcpServer.h"
#include "BaseServer/HttpServer.h"

EventThread* 	g_event_thread 		= NULL;
TaskThreadPool* g_task_thread_pool 	= NULL;
TimerThread*	g_timer_thread		= NULL;
FilesMaster*	g_files_master		= NULL;

int start_thread_event()
{
	int ret = 0;
	
	g_event_thread = new EventThread();
	g_event_thread->Init();
	g_event_thread->Start();
	
	return ret;
}

int start_thread_workers()
{
	int ret = 0;
	
	g_task_thread_pool = new TaskThreadPool();
	g_task_thread_pool->Init();

	return ret;
}

int start_thread_timer()
{
	int ret = 0;

	g_timer_thread = new TimerThread();
	g_timer_thread->Start();

	return ret;
}


int start_files_master()
{
	int ret = 0;

	g_files_master = new FilesMaster();	
	
	return ret;
}

int start_server()
{
	int ret = 0;

	signal(SIGPIPE, SIG_IGN);
	
	u_int32_t ip = 0;
	u_int16_t port = 9191;
	TcpServer*	serverp = new TcpServer();
	ret = serverp->Init(ip, port);
	if(ret < 0)
	{
		fprintf(stderr, "%s: server init error, return %d\n", __FUNCTION__, ret);
	}
	

	u_int32_t ip2 = 0;
	u_int16_t port2 = 9292;
	HttpServer* server2p = new HttpServer();
	ret = server2p->Init(ip2, port2);
	if(ret < 0)
	{
		fprintf(stderr, "%s: server init error, return %d\n", __FUNCTION__, ret);
	}
	
	return ret;
}

int main(int argc, char* argv[])
{
	// 0). main thread, start other thread, then do nothing.
	// 1). 1 events thread.
	// 2). N worker thread.
	// 3). 1 timer thread.
	// 4). start server
	// 5). do nothing forever
	
	prctl(PR_SET_NAME, "oneserver_main");
	
	int ret = 0;
	ret = start_thread_event();
	ret = start_thread_workers();
	ret = start_thread_timer();
	ret = start_files_master();
	ret = start_server();

	while(1)
	{
		sleep(10);
	}	
	
	return ret;
}

