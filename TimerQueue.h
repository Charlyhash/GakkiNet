#ifndef __TIMER_QUEUE_H__
#define __TIMER_QUEUE_H__

#include <memory>
#include <map>
#include <functional>

#include "Mutex.h"
#include "Timer.h"
#include "Event.h"

using namespace std;
/*
TimerQueue是对事件队列的封装
*/
namespace GakkiNet
{
	class EventLoop;

	class TimerQueue
	{
	public:
		TimerQueue(EventLoop* eventLoop);
		~TimerQueue();
		//
		void addOniceTimer(function<void()> & handle, uint32_t interval);
		void addEveryTimer(function<void()> & handle, uint32_t interval);
		void runOniceAfter(function<void()> & handle, int interval);
		void runEveryInterval(function<void()> & handle, int interval);

	private:
		//创建时间的fd
		int createTimeFd();
		//时间到期后的处理：对于每一个过期时间调用对应的回调函数
		void timerHandle();
		//判断是否需要重启时间定时器：新增的定时时间小于原来的过期时间
		bool needResetTimer(std::multimap<uint64_t, shared_ptr<Timer>> & times, shared_ptr<Timer> timer);
		//重启定时器时间：把timerFd的时间设置为最短的过期时间
		void resetTimer(shared_ptr<Timer> timer);
		//重启定时器
		void resetTimer();
		//读取timerFd，为了防止一直触发，必须读走
		void readTimerfd();

	private:
		EventLoop* loop; //所属的eventloop
		int timerFd; //时间的fd,统一到epoll中
		shared_ptr<Event> event; //关注fd的事件类
		MutexLock mutex;
		std::multimap<uint64_t, shared_ptr<Timer>> everyTimers; //存放过期时间-时间类
		std::multimap<uint64_t, shared_ptr<Timer>> oniceTimers; //
	};
}

#endif
