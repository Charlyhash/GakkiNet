#ifndef __SOCKET_ADDR_H__
#define __SOCKET_ADDR_H__
/*
对地址的封装
*/
#include <netinet/in.h>
#include <iostream>

namespace GakkiNet
{
	class SocketAddr
	{
	public:
		//默认构造函数
		SocketAddr();
		//构造函数，使用端口，ip为任意的
		explicit SocketAddr(bool isAny, int port = 0);
		//使用sockaddr_in结构体初始化
		SocketAddr(struct sockaddr_in addr);
		//使用ip:port的string形式
		SocketAddr(const std::string& addrPort);
		//使用addr+port
		SocketAddr(const std::string& addr, uint16_t port);
		//只用port
		SocketAddr(uint16_t port);
		//析构函数
		~SocketAddr();
		//使用sockaddr_in设置ip
		void setAddr(struct sockaddr_in addr);
		//获取地址
		struct sockaddr_in getAddr() const;
		//获取地址的指针
		struct sockaddr_in* getAddrPtr();
		//是否有效
		bool isValid();
		//转为string
		std::string toString() const;

	private:
		bool valid; //是否有效
		struct sockaddr_in sockAddr; //地址
	};
}

#endif