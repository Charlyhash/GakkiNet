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
TimerQueue�Ƕ��¼����еķ�װ
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
		//����ʱ���fd
		int createTimeFd();
		//ʱ�䵽�ں�Ĵ�������ÿһ������ʱ����ö�Ӧ�Ļص�����
		void timerHandle();
		//�ж��Ƿ���Ҫ����ʱ�䶨ʱ���������Ķ�ʱʱ��С��ԭ���Ĺ���ʱ��
		bool needResetTimer(std::multimap<uint64_t, shared_ptr<Timer>> & times, shared_ptr<Timer> timer);
		//������ʱ��ʱ�䣺��timerFd��ʱ������Ϊ��̵Ĺ���ʱ��
		void resetTimer(shared_ptr<Timer> timer);
		//������ʱ��
		void resetTimer();
		//��ȡtimerFd��Ϊ�˷�ֹһֱ�������������
		void readTimerfd();

	private:
		EventLoop* loop; //������eventloop
		int timerFd; //ʱ���fd,ͳһ��epoll��
		shared_ptr<Event> event; //��עfd���¼���
		MutexLock mutex;
		std::multimap<uint64_t, shared_ptr<Timer>> everyTimers; //��Ź���ʱ��-ʱ����
		std::multimap<uint64_t, shared_ptr<Timer>> oniceTimers; //
	};
}

#endif
