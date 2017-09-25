#ifndef __EPOLL_H__
#define __EPOLL_H__
/*
对linux epoll的封装,包含一个epollFd.每个epoll都属于一个event
*/
#include "NoCopyAble.h"
#include <sys/epoll.h>
#include "Event.h"

namespace GakkiNet
{
	class Epoll :NoCopyAble
	{
	public:
		//构造函数
		Epoll();
		//析构函数
		~Epoll();
		//添加事件
		bool addEvent(Event* event);
		//移除
		bool removeEvent(Event* event);
		//修改
		bool modifyEvent(Event* evnet);
		//移除
		bool removeEvent(int fd);
		//等待
		int waitEvent(struct epoll_event* eventList, int eventSize, int timeMs);

	private:
		//对fd进行操作
		int epollCtrl(int op, int fd, int events);
		int epollFd; //epoll的fd,构造函数时使用epoll_create产生
	};

}

#endif
