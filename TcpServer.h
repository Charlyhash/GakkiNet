#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <functional>
#include <memory>
#include <map>
#include <string>

#include "SocketAddr.h"

using namespace std;

namespace GakkiNet
{
	class EventLoop;
	class TcpConnect;
	class Buffer;
	class TcpAccept;
	class EventLoopThreadPool;

	class TcpServer
	{
	public:
		typedef function<void (EventLoop*)> ThreadInitCallback;
		typedef function<void(shared_ptr<TcpConnect>)> ConnectionCallback;
		typedef function<void(shared_ptr<TcpConnect>)> CloseCallback;
		typedef function<void(shared_ptr<TcpConnect>)> WriteCompleteCallback;
		typedef function<void(shared_ptr<TcpConnect>, Buffer&)> MessageCallback;

		TcpServer(EventLoop* loop,SocketAddr& addr);
		~TcpServer();

		void setThreadNum(int num);
		void setThreadInitCallback( ThreadInitCallback cb)
		{
			threadInitCallback_ = cb;
		}
		void setConnectionCallback( ConnectionCallback cb)
		{
			connectionCallback_ = cb;
		}
		void setMessageCallback( MessageCallback cb)
		{
			messageCallback_ = cb;
		}
		void setWriteCompleteCallback( WriteCompleteCallback cb)
		{
			writeCompleteCallback_ = cb;
		}
		void setCloseCallback(CloseCallback cb)
		{
			closeCallback_ = cb;
		}

		void start(); //开启tcp server
		EventLoop* getLoop() const { return loop_; }




		//void connectCallback(shared_ptr<TcpConnect> tcpConnect);
		//void messageCallback(shared_ptr<TcpConnect>, Buffer&);
		//void writeCompletCallback(shared_ptr<TcpConnect> tcpConnect);
		//void connectCloseCallback(shared_ptr<TcpConnect>);

		//void addConnect(std::string name,shared_ptr<TcpConnect> connect);
		//void addConnect(std::string name,TcpConnect* connect);
		//bool haveConect(std::string name);
		//void removeConnect(std::string name);
		//long getConnectCount();

		//void write(TcpConnect& connect,void* data,uint32_t length);
		//void write(std::string& name,void* data,uint32_t length);
		//void write(shared_ptr<TcpConnect> connect,void* data,uint32_t length);
		//void write(std::string& name,std::string& data);
		
	private:
		void newConnected(int sockfd, const SocketAddr& addr);
		void connectCloseEvent(shared_ptr<TcpConnect> connect);
		void removeConnectInLoop(shared_ptr<TcpConnect>& conn);

		EventLoop* loop_;
		SocketAddr addr_;
		shared_ptr<TcpAccept> acceptor_;
				
		bool isStart_;
		int nextConnId_; 

		shared_ptr<EventLoopThreadPool> threadPool_; //持有的thread poll对象
		map<string, shared_ptr<TcpConnect>> connections_; //连接列表

		ThreadInitCallback threadInitCallback_;
		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		WriteCompleteCallback writeCompleteCallback_;
		CloseCallback closeCallback_;



		long getConnectedCount() { return connections_.size(); }
		void addConnect(string name, shared_ptr<TcpConnect> conn);
		void addConnect(string name, TcpConnect* conn);
		bool haveConnect(string name);
		void removeConnect(string name);

	};
}
#endif
