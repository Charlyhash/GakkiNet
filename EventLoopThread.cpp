#include "EventLoopThread.h"
#include <memory>

using namespace GakkiNet;
using namespace std;


EventLoopThread::EventLoopThread(const ThreadInitCallback& cb)
	:loop_(NULL),
    mutex_(),
    condtion_(mutex_),
	thread_(bind(&EventLoopThread::threadFunc, this)),
	callback_(cb)
{

}

EventLoopThread::~EventLoopThread()
{
	thread_.join();
}

EventLoop* EventLoopThread::startLoop()
{
	thread_.start();
    {
        MutexLockGuard lock(mutex_);
        //需要阻塞等待线程初始化
        while(NULL == loop_)
        {
            //如果线程执行到这里还没有wait，loop被初始化了，并执行类notify，那么
			//这里一直阻塞，所以需要加锁。
            condtion_.wait();
        }
    }
    return loop_;
}


void EventLoopThread::threadFunc()
{

	EventLoop loop;
	if (callback_)
		callback_(&loop);

	{
		MutexLockGuard lock(mutex_);
		loop_ = &loop;
		condtion_.notify();
	}

	loop.run();
}