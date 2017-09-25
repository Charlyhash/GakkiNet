#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"

using namespace GakkiNet;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop)
	:mainLoop_(loop),
	started_(false),
	threadNum_(0),
	next_(0)
{}

EventLoopThreadPool::~EventLoopThreadPool()
{}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
	started_ = true;

	for (int i = 0; i < threadNum_; ++i)
	{
		shared_ptr<EventLoopThread> t(new EventLoopThread(cb));
		threads_.push_back(t);
		eventLoopPool_.push_back(t->startLoop());
		//启动eventloopthread线程，调用回调函数
	}

	if (threadNum_ == 0 && cb)
		cb(mainLoop_);
}
EventLoop* EventLoopThreadPool::getNextLoop()
{
	EventLoop* loop = mainLoop_;

	if (!eventLoopPool_.empty())
	{
		loop = eventLoopPool_[next_];
		++next_;
		if (static_cast<size_t>(next_) >= eventLoopPool_.size())
			next_ = 0;
	}

	return loop;
}