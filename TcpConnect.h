#ifndef __TCP_CONNECT_H__
#define __TCP_CONNECT_H__


#include <string>
#include <memory>
#include <functional>

#include <stdint.h>

#include "EventLoop.h"
#include "Buffer.h"
#include "Socket.h"
#include "SocketAddr.h"

using namespace std;

namespace GakkiNet
{
	class TcpConnect :public enable_shared_from_this<TcpConnect>
	{
	public:
		typedef function<void(shared_ptr<TcpConnect> )> ConnectionCallback;
		typedef function<void(shared_ptr<TcpConnect> )> CloseCallback;
		typedef function<void(shared_ptr<TcpConnect> )> WriteCompleteCallback;
		typedef function<void(shared_ptr<TcpConnect>, Buffer&)> MessageCallback;

		TcpConnect(EventLoop* l, struct sockaddr_in addr, int fd);
		~TcpConnect();
		void setMessageCallback(MessageCallback cb) 
		{
			messageCallback = cb;
		}
		void setCloseCallback(CloseCallback cb)
		{
			closeCallback = cb; 
		}
		void setWriteCompletCallback(WriteCompleteCallback cb)
		{
			writeCompleteCallback = cb;
		}
		void setConnectionCallback(ConnectionCallback cb)
		{
			connectionCallback = cb;
		}

		const SocketAddr& getAddr() const;

		string getName() const;

		void write(const char* data);
		void write(const std::string& data);
		void writeData(const void* data, uint32_t length);
		void writeInLoop(const void* data, uint32_t len);

		void connectedHandle();
		void removedHandle();

		void setNoDelay(bool enable);
		void shutdownWrite();

		EventLoop* getLoop() { return loop; }
	private:
		EventLoop* loop;
		SocketAddr socketAddr;
		string name;
		shared_ptr<Socket> socket;
		shared_ptr<Event> event;

		void readEvent();
		void closeEvent();
		void writeEvent();
		void errorEvent();
		
		ConnectionCallback connectionCallback;
		MessageCallback messageCallback;
		CloseCallback closeCallback;
		WriteCompleteCallback writeCompleteCallback;

		Buffer readBuf;
		Buffer writeBuf;
		TcpConnect& getRefer();
	
		int state;
		enum ConnectState { Disconnected, Connecting, Connected, Disconnecting };

	};
}
#endif
