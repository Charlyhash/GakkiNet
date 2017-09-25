#ifndef __TCP_ACCEPT_H__
#define __TCP_ACCEPT_H__

#include "SocketAddr.h"
#include "SocketOperation.h"

#include <memory>
#include <functional>
using namespace std;

namespace GakkiNet
{
	class EventLoop;
	class Socket;
	class Event;

	class TcpAccept
	{
	public:
		typedef function<void(int sockfd, const SocketAddr&)> connCallBack;
		TcpAccept(EventLoop* loop, SocketAddr& addr);
		~TcpAccept();
		void listen();
		bool isListen() const { return listening; }
		void setNewConnectCallback(connCallBack cb)
		{
			newConnectCallback = cb;
		}
	private:
		EventLoop* eventLoop;
		shared_ptr<Socket> socket;
		shared_ptr<Event> event;
		bool listening;
		void acceptHandle();
		connCallBack newConnectCallback;

	};

}
#endif
