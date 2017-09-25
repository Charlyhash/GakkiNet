#include "Event.h"
#include <sys/epoll.h>
#include <unistd.h>
#include "EventLoop.h"

using namespace GakkiNet;
using namespace std;

const int Event::noneEventFlag = 0; //不关注任何事件
const int Event::readEventFlag = EPOLLIN | EPOLLPRI; //可读关注的事件
const int Event::writeEventFlag = EPOLLOUT; //可写关注的事件
const int Event::errorEventFlag = EPOLLERR; //错误关注的事件

Event::Event(EventLoop* loop, int fd) 
	: eventLoop(loop), eventFd(fd), events(0)
{}

Event::~Event() {}

int Event::getFd() { return eventFd; }

uint32_t Event::getEvents() { return events; }

void Event::enableReading(bool isEnable)
{
	if (isEnable)
		events |= readEventFlag;
	else
		events &= ~readEventFlag;
	update();
}

void Event::enableWriting(bool isEnable)
{
	if (isEnable)
		events |= writeEventFlag;
	else
		events &= ~writeEventFlag;
	update();
}

void Event::enableErrorEvent(bool isEnable)
{
	if (isEnable)
		events |= errorEventFlag;
	else
		events &= ~errorEventFlag;
	update();
}

void Event::disableAll()
{
	events = noneEventFlag;
	update();
}

bool Event::isWriting()
{
	return events & writeEventFlag;
}

bool Event::isReading()
{
	return events & readEventFlag;
}

void Event::removeFromLoop()
{
	eventLoop->removeEvent(eventFd);
}

void Event::handle(uint32_t revents)
{
	if ((revents & EPOLLHUP) && !(revents & EPOLLIN))
	{
		if (closeHandle)
			closeHandle();
	}

	if (revents & EPOLLERR)
	{
		if (errorHandle)
			errorHandle();
	}
	if (revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
	{
		if (readHandle)
			readHandle();
	}
	if (revents & EPOLLOUT)
	{
		if (writeHandle)
			writeHandle();
	}
}

void Event::update()
{
	eventLoop->modifyEvent(eventFd);
}
