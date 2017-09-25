#include "EventLoop.h"
#include "GakkiLog.h"
#include "Thread.h"
#include <memory>
#include <functional>
#include <sys/types.h>
#include <signal.h>
#include <sys/eventfd.h>
#include "Event.h"

using namespace GakkiNet;
using namespace std;

int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0)
	{
		LOG(ERROR) << "create event fd";
	}
	return evtfd;
}

//避免接收到sigpipe信号而退出
class IgnorSigPipe
{
public:
	static IgnorSigPipe* getInstance()
	{
		return singnal;
	}
private:
	IgnorSigPipe()
	{
		::signal(SIGPIPE, SIG_IGN);
	}
	static IgnorSigPipe* singnal;
};

IgnorSigPipe* IgnorSigPipe::singnal = new IgnorSigPipe();

const int EventLoop::PollTimeMs = 3000;

EventLoop::EventLoop() 
	: mutex(), 
	eventCtrl(new EventCtrl(this)),
	threadId(CurrentThread::tid()),
	timerQueue(new TimerQueue(this)),
	wakeupFd(createEventfd()),
	wakeupEvent(new Event(this, wakeupFd)),
	callingFunctions(false)
{
	wakeupEvent->setReadFunc(bind(&EventLoop::handleRead, this));
	wakeupEvent->enableReading(true);
}

EventLoop::~EventLoop()
{
	::close(wakeupFd);
}

void EventLoop::addEvent(shared_ptr<Event> event)
{
	eventCtrl->addEvent(event);
}

void EventLoop::removeEvent(shared_ptr<Event> event)
{
	eventCtrl->deleteEvent(event);
}

void EventLoop::modifyEvent(shared_ptr<Event> event)
{
	eventCtrl->modifyEvent(event);
}

void EventLoop::modifyEvent(int fd)
{
	eventCtrl->modifyEvent(fd);
}

void EventLoop::removeEvent(int fd)
{
	eventCtrl->deleteEvent(fd);
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(wakeupFd, &one, sizeof one);
	if (n != sizeof one)
		LOG(ERROR) << "EventLoop::wakeup() writes error";
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = ::read(wakeupFd, &one, sizeof one);
	if (n != sizeof one)
		LOG(ERROR) << "EventLoop::handleRead() reads error";
}

void EventLoop::run()
{
	//判断是否处于当前线程
	if (!inThisThread())
	{
		threadId = CurrentThread::tid();
		LOG(ERROR) << "Loop create and run in different thread";
	}

	while (true)
	{
		LOG(INFO) << "start loop...";
		eventCtrl->waitAndRunHandle(PollTimeMs);
		runAllFunctionInLoop();
	}

}

void EventLoop::runInLoop(function<void()> func)
{
	if (inThisThread())
		func();
	else
		addFunInLoop(func);
}

void EventLoop::runOniceAfter(function<void()> callback, uint32_t interval)
{
	timerQueue->runOniceAfter(callback, interval);
}
void EventLoop::runEveryInterval(function<void()> callback, uint32_t interval)
{
	timerQueue->runEveryInterval(callback, interval);
}

void EventLoop::addFunInLoop(function<void()> func)
{
	{
		MutexLockGuard lock(mutex);
		functions.push_back(func);
	}

	if (!inThisThread() || !callingFunctions)
		wakeup();
}

bool EventLoop::inThisThread()
{
	return (threadId == CurrentThread::tid());
}
void EventLoop::runAllFunctionInLoop()
{
	std::vector<function<void()>> allfunctors;
	callingFunctions = true;
	{
		MutexLockGuard lock(mutex);
		allfunctors.swap(functions);
	}
	
	for (size_t i = 0; i < allfunctors.size(); ++i)
		allfunctors[i]();
	
	callingFunctions = false;
}

