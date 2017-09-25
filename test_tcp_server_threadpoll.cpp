#include "TcpServer.h"
#include "EventLoop.h"
#include "SocketAddr.h"
#include "TcpConnect.h"
#include "CurrentThread.h"
#include "GakkiLog.h"

#include <memory>
#include <functional>
#include <string>
#include <iostream>

#include <stdio.h>

using namespace std;
using namespace GakkiNet;
using namespace std::placeholders;

class TestServer
{
public:
	typedef shared_ptr<TcpConnect> TcpConnectPtr;

	TestServer(EventLoop* loop, SocketAddr& addr)
		:loop_(loop),
		server_(loop, addr)
	{
		server_.setConnectionCallback(std::bind(&TestServer::onConnection, this, _1));
		server_.setMessageCallback(std::bind(&TestServer::OnMessage, this, _1, _2));
		server_.setWriteCompleteCallback(std::bind(&TestServer::onWriteCompleteCallback, this, _1));
		server_.setCloseCallback(std::bind(&TestServer::onCloseCallback, this, _1));
	}
	void setThreadNum(int numThreads)
	{
		server_.setThreadNum(numThreads);
	}

	void start()
	{
		server_.start();
	}

private:

	void onConnection(const TcpConnectPtr& conn)
	{
		cout << "new connect:" << conn->getAddr().toString() << endl;
	}

	void OnMessage(const TcpConnectPtr& conn, Buffer& buf)
	{
		cout << "thread id:" << CurrentThread::tid() << endl;
		string addr = conn->getAddr().toString();
		string data;
		buf.readAllAsString(data);
		cout << "receive data from " << addr << ":" << data << endl;
		conn->write(data);
	}

	void onWriteCompleteCallback(const TcpConnectPtr& conn)
	{
		cout << "thread id:" << CurrentThread::tid() << endl;
		string addr = conn->getAddr().toString();
		cout << addr << ": write compete" << endl;
	}
	void onCloseCallback(const TcpConnectPtr& conn)
	{
		string addr = conn->getAddr().toString();
		cout << "close connect :" << addr << endl;
	}

	EventLoop* loop_;
	TcpServer server_;
};


int main()
{
	LOG(INFO) << "tid :" + to_string(CurrentThread::tid());
	EventLoop loop;
	SocketAddr addr(true, 7777);
	TestServer server(&loop, addr);
	server.setThreadNum(4);
	//server.
	server.start();
	loop.run();
}