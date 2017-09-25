#ifndef __SOCKET_OPERATION_H__
#define __SOCKET_OPERATION_H__
/*
��socket��һЩ�����ķ�װ
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
		//����������socket
		static int createNonblockingSocket();
		//bind��װ, �󶨵�addr
		static int bind(int sockfd, const struct sockaddr_in* addr);
		//����
		static int listen(int sockfd);
		//connet���Է���ַaddr
		static int connect(int sockfd, const struct sockaddr* addr);
		//��������
		static int accept(int sockfd, struct sockaddr_in* addr);
		//read��buffer��
		static ssize_t read(int sockfd, void *buf, size_t count);
		//readv�ķ�װ
		static ssize_t readv(int sockfd, struct iovec *iov, int iovcnt);
		//write�ķ�װ,�����緢��
		static ssize_t write(int sockfd, const void *buf, size_t count);
		//�ر�sockfd
		static void close(int sockfd);
		//�ر�д
		static int shutdownWrite(int sockfd);
		//����NoDelay
		static void setTcpNoDelay(int fd, bool isEnable);
		//���ݶ˿ڷ���addrIn
		static void getAddrAnyIpv4(struct sockaddr_in& addrIn, uint16_t port);
		//stirng��ʽ��ip�Ͷ˿�תΪsockaddr_in�Ľṹ����ʽ 192.168.2.22:6789 ==>sockaddr_in
		static bool toAddrIpv4(const std::string& addrIp, struct sockaddr_in& addrIn);
		//string��ʽ��ip��int��portתΪsockaddr_in�Ľṹ����ʽ 192.168.2.22 6789 ==>sockaddr_in
		static bool toAddrIpv4(const std::string& addr, uint16_t port, struct sockaddr_in& addrIn);
		//portתΪscokaddr_in�Ľṹ����ʽ������IpΪ�κ�IP
		static bool toAddrIpv4(uint16_t port, struct sockaddr_in& addrIn);
		//sockaddr_inתΪstring��ʽ 192.168.2.2:6789
		static std::string ipToString(struct sockaddr_in addr);
		//sockaddr_inתΪstring��ʽ 192.168.2.2:6789
		static std::string toString(struct sockaddr_in addr);
		//stringתΪint
		template<typename T>
		static bool stringToInt(const std::string& str, T& num);
		//�ַ��ָ�string
		static std::vector<std::string> split(const std::string& str, char delim);
	};
}

#endif