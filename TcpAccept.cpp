#include "TcpAccept.h"
#include "Event.h"
#include "EventLoop.h"
#include "Socket.h"
#include "SocketAddr.h"

#include <memory>
#include <functional>

#include "stdio.h"

using namespace std;
using namespace GakkiNet;

TcpAccept::TcpAccept(EventLoop* loop, SocketAddr& addr)
	: eventLoop(loop),
	socket(new Socket(SocketOperation::createNonblockingSocket())),
	event(new Event(loop, socket->getFd())),
	listening(false)
{
	loop->addEvent(event);
	socket->bind(addr);
	event->setReadFunc(bind(&TcpAccept::acceptHandle, this));
}

TcpAccept::~TcpAccept()
{
	event->disableAll();
	event->removeFromLoop();
}

void TcpAccept::listen()
{
	socket->listen();
	event->enableReading(true);
}

void TcpAccept::acceptHandle()
{
	SocketAddr connectAddr;
	int fd;
	if ((fd = socket->accept(connectAddr)) > 0)
	{
		if (newConnectCallback)
			newConnectCallback(fd, connectAddr);
		else
			::close(fd);
	}
}