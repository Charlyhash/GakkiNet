#include "Epoll.h"
#include "GakkiLog.h"
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
using namespace GakkiNet;

Epoll::Epoll() : epollFd(epoll_create1(EPOLL_CLOEXEC))
{
	if (epollFd < 0)
		LOG(ERROR) << "epll_create1 error";
}

Epoll::~Epoll()
{
	::close(epollFd);
}

bool Epoll::addEvent(Event* event)
{
	if (epollCtrl(EPOLL_CTL_ADD, event->getFd(), event->getEvents()) < 0)
	{
		LOG(ERROR) << "add epoll";
		return false;
	}
	return true;
}
bool Epoll::removeEvent(Event* event)
{
	if (epollCtrl(EPOLL_CTL_DEL, event->getFd(), event->getEvents()) < 0)
	{
		LOG(ERROR) << "delete epoll";
		return false;
	}
	return true;
}
bool Epoll::modifyEvent(Event* event)
{
	if (epollCtrl(EPOLL_CTL_MOD, event->getFd(), event->getEvents()) < 0)
	{
		LOG(ERROR) << "modify epoll";
		return false;
	}
	return true;
}
bool Epoll::removeEvent(int fd)
{
	if (epollCtrl(EPOLL_CTL_DEL, fd, 0) < 0)
	{
		LOG(ERROR) << "delete epoll";
		return false;
	}
	return true;
}
int Epoll::waitEvent(struct epoll_event* eventList, int eventSize, int timeMs)
{
	int rst = epoll_wait(epollFd, eventList, eventSize, timeMs);

	return rst;
}

int Epoll::epollCtrl(int op, int fd, int events)
{
	struct epoll_event event;
	bzero(&event, sizeof(event));
	event.events = events;
	event.data.fd = fd;
	//修改会反映在events上
	int rst = epoll_ctl(epollFd, op, fd, &event);
	
	return rst;
}