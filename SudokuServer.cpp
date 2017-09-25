#include "suduku.h"
#include "TcpServer.h"
#include "EventLoop.h"
#include "SocketAddr.h"
#include "TcpConnect.h"
#include "CurrentThread.h"
#include "ThreadPool.h"
#include "GakkiLog.h"

#include <memory>
#include <functional>
#include <string>
#include <iostream>
#include <assert.h>

#include <stdio.h>

using namespace GakkiNet;
using namespace std;
using namespace std::placeholders;

class SudokuServer
{
public:

	typedef shared_ptr<TcpConnect> TcpConnectPtr;

	SudokuServer(EventLoop* loop, SocketAddr& addr)
		: loop_(loop),
		server_(loop, addr)
	{
		server_.setConnectionCallback(std::bind(&SudokuServer::onConnection, this, _1));
		server_.setMessageCallback(std::bind(&SudokuServer::OnMessage, this, _1, _2));
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
		assert(data.size() >= 81);
		string request = data.substr(0, 81);
		cout << "receive data from " << addr << ":" << data << endl;
		LOG(INFO) << "request:" + request;
		string ret = solveSuduku(request);
		ret += "\r\n";
		conn->write(ret);
	}

	EventLoop* loop_;
	TcpServer server_;
};

int main()
{
	LOG(INFO) << "tid :" + to_string(CurrentThread::tid());
	EventLoop loop;
	SocketAddr addr(true, 7777);
	SudokuServer server(&loop, addr);
	server.setThreadNum(4);
	server.start();
	loop.run();
}