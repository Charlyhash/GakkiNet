#ifndef __EVENT_LOOP_THREAD_H__
#define __EVENT_LOOP_THREAD_H__

#include "Thread.h"
#include "Condition.h"
#include "EventLoop.h"

#include <memory>

/*
I/O线程类，对I/O线程的封装
*/

namespace GakkiNet
{
	class EventLoopThread
	{
	public:
		typedef function<void(EventLoop*)> ThreadInitCallback;
		//构造函数，传入需要运行的回调函数
		EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
		~EventLoopThread();
		//开启I/O线程
		EventLoop* startLoop();
	private:
		EventLoop* loop_; //所属event loop
		MutexLock mutex_; 
		Condition condtion_;
		Thread thread_; //包含一个线程独享
		ThreadInitCallback callback_; //线程的回调函数
		void threadFunc(); //线程函数
	};
}

#endif
