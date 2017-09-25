#include "Socket.h"
#include "SocketOperation.h"
#include "GakkiLog.h"

using namespace GakkiNet;

Socket::Socket(int fd) : socketFd(fd) {}

Socket::~Socket()
{
	SocketOperation::close(socketFd);
}

int Socket::getFd()
{
	return socketFd;
}

void Socket::bind(SocketAddr& addr)
{
	setTcpNoDelay(true);
	int optval = 1;
  	::setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));
  	::setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval));
	SocketOperation::bind(socketFd, addr.getAddrPtr());
}

int Socket::accept(SocketAddr& addr)
{
	struct sockaddr_in clientAddr;
	int rst = SocketOperation::accept(socketFd, &clientAddr);
	if (rst > 0)
		addr.setAddr(clientAddr);
	return rst;
}

void Socket::listen()
{
	SocketOperation::listen(socketFd);
}

void Socket::setTcpNoDelay(bool enable)
{
	SocketOperation::setTcpNoDelay(socketFd, enable);
}


int Socket::shutdownWrite()
{
	return SocketOperation::shutdownWrite(socketFd);
}