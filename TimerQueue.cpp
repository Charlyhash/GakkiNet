#include "TimerQueue.h"
#include "EventLoop.h"
#include "GakkiLog.h"
#include "sys/timerfd.h"

using namespace GakkiNet;
using namespace std;

TimerQueue::TimerQueue(EventLoop* eventLoop)
	:loop(eventLoop),
	timerFd(createTimeFd()),
	event(new Event(loop, timerFd)),
	mutex()
{}

TimerQueue::~TimerQueue()
{
	::close(timerFd);
}

void TimerQueue::addOniceTimer(function<void()> & handle, uint32_t interval)
{
	shared_ptr<Timer> timer(new Timer(interval, handle));
	oniceTimers.insert(pair<uint64_t, shared_ptr<Timer> >(timer->getTimeOutMSecond(), timer));
	if (needResetTimer(oniceTimers, timer) && needResetTimer(everyTimers, timer))
	{
		resetTimer(timer);
	}
}

void TimerQueue::addEveryTimer(function<void()> & handle, uint32_t interval)
{
	shared_ptr<Timer> timer(new Timer(interval, handle));
	everyTimers.insert(pair<uint64_t, shared_ptr<Timer> >(timer->getTimeOutMSecond(), timer));
	if (needResetTimer(oniceTimers, timer) && needResetTimer(everyTimers, timer))
	{
		resetTimer(timer);
	}
}

void TimerQueue::runOniceAfter(function<void()> & handle, int interval)
{
	MutexLockGuard lock(mutex);
	addOniceTimer(handle, interval);
}

void TimerQueue::runEveryInterval(function<void()> & handle, int interval)
{
	MutexLockGuard lock(mutex);
	addEveryTimer(handle, interval);
}

int TimerQueue::createTimeFd()
{
	int fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (fd < 0)
	{
		LOG(ERROR) << "failed to create time fd";
	}
	return fd;
}

void TimerQueue::timerHandle()
{
	MutexLockGuard lock(mutex);
	readTimerfd();
	multimap<uint64_t, shared_ptr<Timer> >::iterator it;

	//mapÊÇ»ùÓÚÆ½ºâÊ÷ÊµÏÖ£¬ËùÒÔÖ»Ðè±éÀúµ½µ±Ç°´¥·¢¶¨Ê±Æ÷£¬ÍË³ö¼´¿É¡£
	for (it = oniceTimers.begin(); it != oniceTimers.end(); it++)
	{
		if (it->first > Timer::getNowTimeMSecond())
		{
			break;
		}
		it->second->timerHandle();
		oniceTimers.erase(it);
	}


	for (it = everyTimers.begin(); it != everyTimers.end(); it++)
	{
		if (it->first > Timer::getNowTimeMSecond())
		{
			break;
		}
		it->second->timerHandle();
		shared_ptr<Timer> timer = it->second;
		timer->update();
		everyTimers.insert(pair<uint64_t, shared_ptr<Timer> >(timer->getTimeOutMSecond(), timer));
		everyTimers.erase(it);
	}
	resetTimer();
}

bool TimerQueue::needResetTimer(std::multimap<uint64_t, shared_ptr<Timer>> & times, shared_ptr<Timer> timer)
{
	if (times.empty())
		return true;

	multimap<uint64_t, shared_ptr<Timer> >::iterator it = times.begin();
	//Èç¹û´¥·¢Ê±¼äÐ¡ÓÚÐÂÔö¶¨Ê±Æ÷Ê±¼ä£¬Ôò²»ÐèÒªÖØÖÃ¶¨Ê±Æ÷¡£
	if (it->first < timer->getTimeOutMSecond())
	{
		return false;
	}
	//Èç¹û¼üÖµÒÑ´æÔÚ£¬Ôò²»ÐèÒªÖØÖÃ¶¨Ê±Æ÷
	if (times.count(timer->getTimeOutMSecond())>1)
	{
		return false;
	}
	return true;
}

void TimerQueue::resetTimer(shared_ptr<Timer> timer)
{
	struct itimerspec newValue;
	struct itimerspec oldValue;
	bzero(&newValue, sizeof newValue);
	bzero(&oldValue, sizeof oldValue);
	newValue.it_value = timer->getTimeInterval();
	int ret = ::timerfd_settime(timerFd, 0, &newValue, &oldValue);
	if (ret < 0)
	{
		LOG(ERROR) << "timerfd_settime() error";
	}
}


void TimerQueue::resetTimer()
{
	if (oniceTimers.empty())
	{
		if (!everyTimers.empty())
		{
			multimap<uint64_t, shared_ptr<Timer> >::iterator it;
			it = everyTimers.begin();
			resetTimer(it->second);
		}
	}
	else
	{
		if (everyTimers.empty())
		{
			multimap<uint64_t, shared_ptr<Timer> >::iterator it;
			it = oniceTimers.begin();
			resetTimer(it->second);
		}
		else
		{
			multimap<uint64_t, shared_ptr<Timer> >::iterator it1;
			multimap<uint64_t, shared_ptr<Timer> >::iterator it2;
			it1 = everyTimers.begin();
			it2 = oniceTimers.begin();
			if (it1->second->getTimeOutMSecond() < it2->second->getTimeOutMSecond())
			{
				resetTimer(it1->second);
			}
			else
			{
				resetTimer(it2->second);
			}
		}
	}
}

void TimerQueue::readTimerfd()
{
	uint64_t cnt;
	int n = ::read(timerFd, &cnt, sizeof cnt);
	if (n != sizeof cnt)
	{
		LOG(ERROR) << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
	}
}