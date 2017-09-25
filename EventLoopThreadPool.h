#ifndef __EVENT_LOOP_THREAD_POOL_H__
#define __EVENT_LOOP_THREAD_POOL_H__

#include <vector>
#include <memory>
#include <functional>
#include "EventLoop.h"
#include "EventLoopThread.h"

namespace GakkiNet
{
	class EventLoopThreadPool
	{
	public:
		typedef std::function<void(EventLoop*)> ThreadInitCallback;

		EventLoopThreadPool(EventLoop* loop);
		~EventLoopThreadPool();
		bool isStarted() { return started_; }
		void setThreadNum(int num) { threadNum_ = num; }
		void start(const ThreadInitCallback& cb = ThreadInitCallback());
		EventLoop* getNextLoop();

	private:
		EventLoop* mainLoop_; //所属主线程
		bool started_; //
		int threadNum_; //
		int next_; //
		std::vector<shared_ptr<EventLoopThread>> threads_; //
		std::vector<EventLoop* > eventLoopPool_; //Eventloop集合
	};
}

#endif
