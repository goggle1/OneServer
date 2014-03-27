#include <stdio.h>

#include "EventHandler.h"

EventHandler::EventHandler()
{
	m_fd = -1;
}

EventHandler::~EventHandler()
{
	// todo:
}

int EventHandler::EnqueEvents(u_int32_t events)
{
	// todo:
	fprintf(stdout, "%s: events=0x%08X\n", __PRETTY_FUNCTION__, events);
	return 0;
}

int EventHandler::Run()
{
	// todo:
	return 0;
}
