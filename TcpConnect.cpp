#include "TcpConnect.h"
#include "GakkiLog.h"
#include "SocketOperation.h"
#include "EventLoop.h"
#include "Buffer.h"
#include "GakkiLog.h"

#include <functional>
#include <string>
#include <memory>

using namespace GakkiNet;
using namespace std;

TcpConnect::TcpConnect(EventLoop* l,struct sockaddr_in addr,int fd)
    :loop(l),
    socketAddr(addr),
    name(socketAddr.toString()),
    socket(new Socket(fd)),
    event(new Event(loop,fd)),
    state(Disconnecting)
{
    setNoDelay(true);
    loop->addEvent(event);
    event->setReadFunc(bind(&TcpConnect::readEvent,this));
    event->setCloseFunc(bind(&TcpConnect::closeEvent,this));
    event->setWriteFunc(bind(&TcpConnect::writeEvent,this));
    event->setErrorFunc(bind(&TcpConnect::errorEvent,this));

    LOG(INFO) << "new connect:" + name;
}

TcpConnect::~TcpConnect()
{

}

void TcpConnect::readEvent()
{
    int error= 0;
    int n = readBuf.readFromIO(event->getFd(), error);
    if (n > 0)
    {
        if(messageCallback)
        {
            messageCallback(shared_from_this(),readBuf);
        }
    }
    else if (n == 0)
    {
        closeEvent();
    }
    else
    {
        LOG(ERROR) << "TcpConnection::handleRead error :"<<error;
    }
}

void TcpConnect::closeEvent()
{
    state = Disconnected;
    if(closeCallback)
    {
        closeCallback(shared_from_this());
    }
}

TcpConnect& TcpConnect::getRefer()
{
    return (*this);
}

const SocketAddr& TcpConnect::getAddr() const
{
    return socketAddr;
}

string TcpConnect::getName() const
{
    return name;
}


void TcpConnect::connectedHandle()
{
    LOG(INFO) << "connect handle...";
    state = Connected;
    event->enableReading(true);
    //epoll为电平触发
    //event->enableWriting(true);
    event->enableErrorEvent(true);
	connectionCallback(shared_from_this());
}

void TcpConnect::removedHandle()
{
	LOG(INFO) << "disconnect...";
	if (state == Connected)
	{
		state = Disconnected;
		if (closeCallback)
			closeCallback(shared_from_this());
		event->disableAll();
	}
	event->removeFromLoop();
}

void TcpConnect::errorEvent()
{
    closeEvent();
}

void TcpConnect::writeEvent()
{
    if (event->isWriting())
    {
        int n = SocketOperation::write(event->getFd(),  writeBuf.readIndexPtr(),writeBuf.readableBytes());
        if (n > 0)
        {
            writeBuf.clearReadIndex(n);
            if (writeBuf.isEmpty())
            {
                event->enableWriting(false);
                if (writeCompleteCallback)
                {
                    shared_ptr<TcpConnect> tmp(this);
                    writeCompleteCallback(tmp);
                    if(writeCompleteCallback)
                        writeCompleteCallback( shared_from_this());
                }
            }
        }
        else
        {
            LOG(ERROR) << "write data error";
        }
    }
    else
    {
		string errMst = "Connection fd = " + std::to_string(event->getFd()) + " is down, no more writing";
		LOG(ERROR) << errMst;
    }
}

void TcpConnect::write(const char* data)
{
    int len = ::strlen(data);
    writeData((void*)data,len);
}

void TcpConnect::write(const string& data)
{
    writeData(&(*data.begin()),data.length());
}

void TcpConnect::writeData(const void* data, uint32_t length)
{
    int n = 0;
    size_t remaining = length;
    bool faultError = false;
    if (state == Disconnected)
    {
		LOG(ERROR) << "disconnected, give up writing";
        return;
    }

    //如该写数据缓冲区内有数据，直接写入socket缓冲区会导致数据交叠
    if (!event->isWriting() && writeBuf.isEmpty())
    {
        n = SocketOperation::write(event->getFd(), data, length);
        if (n >= 0)
        {
            remaining = length - n;
            if (remaining == 0 && writeCompleteCallback)
            {
                writeCompleteCallback(shared_from_this());
            }
        }
        else
        {
            n = 0;
            if (errno != EWOULDBLOCK)
            {
				LOG(ERROR)  << "write data error";
                if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
                {
                    faultError = true;
                }
            }
        }
    }

    if (!faultError && remaining > 0)
    {
        writeBuf.append(static_cast<const char*>(data)+n, remaining);
        if (!event->isWriting())
        {
            event->enableWriting(true);
        }
    }

}

void TcpConnect::writeInLoop(const void* data, uint32_t len)
{
    loop->runInLoop(std::bind(&TcpConnect::writeData,this,data,len));
}

void TcpConnect::setNoDelay(bool enable)
{
    socket->setTcpNoDelay(enable);
}

void TcpConnect::shutdownWrite()
{
    if (state == Connected)
    {
        state = Disconnecting;
        socket->shutdownWrite();
    }
}
