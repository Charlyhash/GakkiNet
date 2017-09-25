#ifndef __EVENT_CTRL_H__
#define __EVENT_CTRL_H__

#include <vector>
#include <map>
#include <memory>
#include "Epoll.h"
#include "Event.h"
/*
对事件操作的类，并在事件循环中等待事件产生，调用对应的回调函数, 该类属于一个eventloop,
供eventloop使用。对事件的操作同事更新到eventloop（eventloop记录了所有的事件），同时需
要更新对应的epoll的监听
*/

namespace GakkiNet
{
	class EventLoop;
	class EventCtrl
	{
	public:
		EventCtrl(EventLoop* lo);
		~EventCtrl();
		//添加事件
		void addEvent(shared_ptr<Event> event);
		//删除事件
		void deleteEvent(shared_ptr<Event> event);
		//修改事件
		void modifyEvent(shared_ptr<Event> event);
		//修改事件
		void modifyEvent(int fd);
		//删除事件
		void deleteEvent(int fd);
		//等待事件循环并调用对应的回调函数
		void waitAndRunHandle(int timeMs);
	private:
		static const int activeEventLength; //活动的事件列表长度
		std::map<int, weak_ptr<Event> > eventPool;//fd到event的映射
		Epoll epoll; //事件注册到这个epoll
		EventLoop* loop; //所属的event loop
		std::vector<struct epoll_event> activeEvents; //活跃的时间
	};
}

#endif 
