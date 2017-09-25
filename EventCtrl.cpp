#include "EventCtrl.h"
#include "GakkiLog.h"

#include <memory>
#include <vector>

using namespace GakkiNet;
using namespace std;

const int EventCtrl::activeEventLength = 16;

EventCtrl::EventCtrl(EventLoop* lo):loop(lo), activeEvents(activeEventLength)
{}

EventCtrl::~EventCtrl()
{
	eventPool.clear();
}

void EventCtrl::addEvent(shared_ptr<Event> event)
{
	eventPool.insert(pair<int, weak_ptr<Event>>(event->getFd(), event));
	epoll.addEvent(event.get());
}
void EventCtrl::deleteEvent(shared_ptr<Event> event)
{
	auto iter = eventPool.find(event->getFd());
	eventPool.erase(iter);
	epoll.removeEvent(event.get());
}
void EventCtrl::modifyEvent(shared_ptr<Event> event)
{
	if (eventPool.find(event->getFd()) != eventPool.end())
	{
		eventPool[event->getFd()] = event;
		epoll.modifyEvent(event.get());
	}
}
void EventCtrl::modifyEvent(int fd)
{
	if (eventPool.find(fd) != eventPool.end())
	{
		shared_ptr<Event> eventS = eventPool[fd].lock();
		if (eventS)
		{
			Event* event = eventS.get();
			epoll.modifyEvent(event);
		}
	}
}
void EventCtrl::deleteEvent(int fd)
{
	if (eventPool.find(fd) != eventPool.end())
	{
		epoll.removeEvent(fd);
		eventPool.erase(fd);
	}
}
void EventCtrl::waitAndRunHandle(int timeMs)
{
	LOG(INFO) << "wait evnets...";
	int cnt = epoll.waitEvent(&*activeEvents.begin(), activeEvents.size(), timeMs);
	if (cnt < 0)
	{
		return;
	}
	for (int i = 0; i < cnt; i++)
	{
		int fd = activeEvents[i].data.fd;
		shared_ptr<Event> ioEvent = eventPool[fd].lock();
		if (ioEvent)
		{
			ioEvent->handle(activeEvents[i].events);
		}
		else
		{
			deleteEvent(fd);
		}
	}
}

