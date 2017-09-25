#include "TcpServer.h"
#include "Event.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "SocketAddr.h"
#include "Buffer.h"
#include "TcpConnect.h"
#include "TcpAccept.h"
#include "GakkiLog.h"

#include <assert.h>
#include <memory>
#include <functional>
#include <string>
#include <map>

using namespace std;
using namespace GakkiNet;
using namespace std::placeholders;

TcpServer::TcpServer(EventLoop* loop, SocketAddr& addr)
	: loop_(loop),
	addr_(addr),
	acceptor_(new TcpAccept(loop, addr)),
	isStart_(false),
	nextConnId_(1),
	threadPool_(new EventLoopThreadPool(loop)),
	connectionCallback_(NULL),
	messageCallback_(NULL)
{
	acceptor_->setNewConnectCallback(bind(&TcpServer::newConnected, this, _1, _2));
	LOG(INFO) << "Create tcp server";
}

TcpServer::~TcpServer()
{

}

void TcpServer::setThreadNum(int num)
{
	assert(num > 0);
	threadPool_->setThreadNum(num);
}


void TcpServer::start()
{
	LOG(INFO) << "start server:" + addr_.toString() ;
	if (!isStart_)
	{
		isStart_ = true;
		threadPool_->start(threadInitCallback_);
	}

	if (!acceptor_->isListen())
	{
		//loop_->runInLoop(bind(TcpAccept::listen, get_pointer(acceptor_)));
		acceptor_->listen();
	}
}

void TcpServer::newConnected(int sockfd, const SocketAddr& addr)
{
	EventLoop* ioLoop = threadPool_->getNextLoop();
	shared_ptr<TcpConnect> tcpConnect(new TcpConnect(ioLoop, addr.getAddr(), sockfd));
	addConnect(addr.toString(), tcpConnect);
	tcpConnect->setConnectionCallback(connectionCallback_);
	tcpConnect->setMessageCallback(messageCallback_);
	tcpConnect->setCloseCallback(bind(&TcpServer::connectCloseEvent, this, _1));
	//tcpConnect->connectedHandle();
	ioLoop->runInLoop(bind(&TcpConnect::connectedHandle, tcpConnect));

	string infoMsg = "new connection: " + addr.toString() + " <cnt>:" + to_string(getConnectedCount());
	LOG(INFO) << infoMsg;

}

void TcpServer::connectCloseEvent(shared_ptr<TcpConnect> connect)
{
	loop_->runInLoop(bind(&TcpServer::removeConnectInLoop, this, connect));
}

void TcpServer::removeConnectInLoop(shared_ptr<TcpConnect>& conn)
{
	//closeCallback_(conn);
	removeConnect(conn->getName());
	EventLoop* ioLoop = conn->getLoop();
	ioLoop->runInLoop(bind(&TcpConnect::removedHandle, conn));
	
	//conn->

}

void TcpServer::addConnect(string name, shared_ptr<TcpConnect> conn)
{
	connections_.insert({ name, conn });
}
void TcpServer::addConnect(string name, TcpConnect* conn)
{
	shared_ptr<TcpConnect> connectPtr(conn);
	addConnect(name, connectPtr);
}
bool TcpServer::haveConnect(string name)
{
	return (connections_.find(name) != connections_.end());
}
void TcpServer::removeConnect(string name)
{
	connections_.erase(name);
}