#ifndef __SOCKET_OPERATION_H__
#define __SOCKET_OPERATION_H__
/*
对socket的一些操作的封装
*/

#include <sys/types.h>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <linux/tcp.h>

namespace GakkiNet
{
	class SocketOperation
	{
	public:
		static const int32_t Ipv4AddrAny;
		//创建非阻塞socket
		static int createNonblockingSocket();
		//bind封装, 绑定到addr
		static int bind(int sockfd, const struct sockaddr_in* addr);
		//监听
		static int listen(int sockfd);
		//connet到对方地址addr
		static int connect(int sockfd, const struct sockaddr* addr);
		//接受连接
		static int accept(int sockfd, struct sockaddr_in* addr);
		//read到buffer中
		static ssize_t read(int sockfd, void *buf, size_t count);
		//readv的封装
		static ssize_t readv(int sockfd, struct iovec *iov, int iovcnt);
		//write的封装,供网络发送
		static ssize_t write(int sockfd, const void *buf, size_t count);
		//关闭sockfd
		static void close(int sockfd);
		//关闭写
		static int shutdownWrite(int sockfd);
		//设置NoDelay
		static void setTcpNoDelay(int fd, bool isEnable);
		//根据端口返回addrIn
		static void getAddrAnyIpv4(struct sockaddr_in& addrIn, uint16_t port);
		//stirng形式的ip和端口转为sockaddr_in的结构体形式 192.168.2.22:6789 ==>sockaddr_in
		static bool toAddrIpv4(const std::string& addrIp, struct sockaddr_in& addrIn);
		//string形式的ip和int的port转为sockaddr_in的结构体形式 192.168.2.22 6789 ==>sockaddr_in
		static bool toAddrIpv4(const std::string& addr, uint16_t port, struct sockaddr_in& addrIn);
		//port转为scokaddr_in的结构体形式，其中Ip为任何IP
		static bool toAddrIpv4(uint16_t port, struct sockaddr_in& addrIn);
		//sockaddr_in转为string形式 192.168.2.2:6789
		static std::string ipToString(struct sockaddr_in addr);
		//sockaddr_in转为string形式 192.168.2.2:6789
		static std::string toString(struct sockaddr_in addr);
		//string转为int
		template<typename T>
		static bool stringToInt(const std::string& str, T& num);
		//字符分割string
		static std::vector<std::string> split(const std::string& str, char delim);
	};
}

#endif