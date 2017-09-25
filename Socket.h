#ifndef __SOCKET_H__
#define __SOCKET_H__

/*
对socket的封装，包含一个socketFd
*/
#include "SocketOperation.h"
#include "SocketAddr.h"
#include "NoCopyAble.h"

namespace GakkiNet
{
	class Socket : public NoCopyAble
	{
	public:
		//构造函数
		explicit Socket(int fd);
		//析构函数
		~Socket();
		//绑定到addr
		void bind(SocketAddr& addr);
		//接受addr
		int accept(SocketAddr& addr);
		//监听
		void listen();
		//获得fd
		int getFd();
		//设置tcp no delay
		void setTcpNoDelay(bool enable);
		//关闭写端
		int shutdownWrite();
	private:
		int socketFd; //socket的文件描述符
	};
}


#endif
