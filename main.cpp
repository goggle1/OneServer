#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <pthread.h>

#include "BaseServer/EventThread.h"
#include "BaseServer/TcpServer.h"
#include "BaseServer/TaskThreadPool.h"

EventThread* 	g_event_thread 		= NULL;
TaskThreadPool* g_task_thread_pool 	= NULL;

int start_thread_event()
{
	int ret = 0;

	g_event_thread = new EventThread();
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

int start_server()
{
	int ret = 0;
	
	u_int32_t ip = 0;
	u_int16_t port = 9191;
	TcpServer*	serverp = new TcpServer();
	ret = serverp->Init(ip, port);
	if(ret < 0)
	{
		fprintf(stderr, "%s: server init error, return %d\n", __FUNCTION__, ret);
	}
	ret = g_event_thread->m_events.AddWatch(serverp->GetFd(), EVENT_READ, serverp);
	if(ret < 0)
	{
		fprintf(stderr, "%s: events AddWatch, return %d\n", __FUNCTION__, ret);
	}
	
	return ret;
}

int main(int argc, char* argv[])
{
	// 0). main thread, start other thread, then do nothing.
	// 1). 1 events thread.
	// 2). N worker thread.
	// 3). 1 timer thread.
	prctl(PR_SET_NAME, "oneserver_main");
	
	int ret = 0;
	ret = start_thread_event();
	ret = start_thread_workers();
	//ret = start_thread_timer();

	ret = start_server();

	while(1)
	{
		sleep(10);
	}	
	
	return ret;
}

