#ifndef __SOCKET_ADDR_H__
#define __SOCKET_ADDR_H__
/*
�Ե�ַ�ķ�װ
*/
#include <netinet/in.h>
#include <iostream>

namespace GakkiNet
{
	class SocketAddr
	{
	public:
		//Ĭ�Ϲ��캯��
		SocketAddr();
		//���캯����ʹ�ö˿ڣ�ipΪ�����
		explicit SocketAddr(bool isAny, int port = 0);
		//ʹ��sockaddr_in�ṹ���ʼ��
		SocketAddr(struct sockaddr_in addr);
		//ʹ��ip:port��string��ʽ
		SocketAddr(const std::string& addrPort);
		//ʹ��addr+port
		SocketAddr(const std::string& addr, uint16_t port);
		//ֻ��port
		SocketAddr(uint16_t port);
		//��������
		~SocketAddr();
		//ʹ��sockaddr_in����ip
		void setAddr(struct sockaddr_in addr);
		//��ȡ��ַ
		struct sockaddr_in getAddr() const;
		//��ȡ��ַ��ָ��
		struct sockaddr_in* getAddrPtr();
		//�Ƿ���Ч
		bool isValid();
		//תΪstring
		std::string toString() const;

	private:
		bool valid; //�Ƿ���Ч
		struct sockaddr_in sockAddr; //��ַ
	};
}

#endif