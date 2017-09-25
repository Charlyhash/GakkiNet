#ifndef __SOCKET_H__
#define __SOCKET_H__

/*
��socket�ķ�װ������һ��socketFd
*/
#include "SocketOperation.h"
#include "SocketAddr.h"
#include "NoCopyAble.h"

namespace GakkiNet
{
	class Socket : public NoCopyAble
	{
	public:
		//���캯��
		explicit Socket(int fd);
		//��������
		~Socket();
		//�󶨵�addr
		void bind(SocketAddr& addr);
		//����addr
		int accept(SocketAddr& addr);
		//����
		void listen();
		//���fd
		int getFd();
		//����tcp no delay
		void setTcpNoDelay(bool enable);
		//�ر�д��
		int shutdownWrite();
	private:
		int socketFd; //socket���ļ�������
	};
}


#endif
