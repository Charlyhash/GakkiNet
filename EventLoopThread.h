#ifndef __EVENT_LOOP_THREAD_H__
#define __EVENT_LOOP_THREAD_H__

#include "Thread.h"
#include "Condition.h"
#include "EventLoop.h"

#include <memory>

/*
I/O�߳��࣬��I/O�̵߳ķ�װ
*/

namespace GakkiNet
{
	class EventLoopThread
	{
	public:
		typedef function<void(EventLoop*)> ThreadInitCallback;
		//���캯����������Ҫ���еĻص�����
		EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
		~EventLoopThread();
		//����I/O�߳�
		EventLoop* startLoop();
	private:
		EventLoop* loop_; //����event loop
		MutexLock mutex_; 
		Condition condtion_;
		Thread thread_; //����һ���̶߳���
		ThreadInitCallback callback_; //�̵߳Ļص�����
		void threadFunc(); //�̺߳���
	};
}

#endif
